/////////////////////////////////////////////////////////////////
// lp-solver
/////////////////////////////////////////////////////////////////
#include <ilcplex/cplex.h>
#include <stdlib.h>
#include <string.h>
#include "CN_Solver.h"

/* Forward declaration for function at end of program */

static int
	buildNetwork (CPXENVptr env, CPXNETptr net, WidgetGraph* widgetNet, 
	double* &supply, int* &head, int* &tail, double* &obj, double* &ub, double* &lb, int opMode);

static void
	free_and_null (char **ptr);

int Solver::lpSolver(int opMode)
{
	/* Declare variables and arrays for retrieving problem data and
	  solution information later on. */
	// cout << "lp solver" << endl;	
	int      narcs;
	int      nnodes;
	int      solstat;
	double   objval;
	double   *x     = NULL;
	double   *pi    = NULL;
	double   *slack = NULL;
	double   *dj    = NULL;

	CPXENVptr env = NULL;
	CPXNETptr net = NULL;
	int       status;
	int status1;
	int       i, j;
	int cnt = 0;

	/* Initialize the CPLEX environment */

	env = CPXopenCPLEX (&status);

	/* If an error occurs, the status value indicates the reason for
	  failure.  A call to CPXgeterrorstring will produce the text of
	  the error message.  Note that CPXopenCPLEX produces no
	  output, so the only way to see the cause of the error is to use
	  CPXgeterrorstring.  For other CPLEX routines, the errors will
	  be seen if the CPXPARAM_ScreenOutput indicator is set to CPX_ON.  */

	if ( env == NULL ) {
	  char  errmsg[CPXMESSAGEBUFSIZE];
	  fprintf (stderr, "Could not open CPLEX environment.\n");
	  CPXgeterrorstring (env, status, errmsg);
	  fprintf (stderr, "%s", errmsg);
	  goto TERMINATE;
	}

	/* Turn on output to the screen */

	status = CPXsetintparam (env, CPX_PARAM_SCRIND, CPX_OFF);
	if ( status ) {
	  fprintf (stderr, 
				"Failure to turn on screen indicator, error %d.\n", status);
	  goto TERMINATE;
	}

	/* Create the problem. */

	net = CPXNETcreateprob (env, &status, "netex1");

	/* A returned pointer of NULL may mean that not enough memory
	  was available or there was some other problem.  In the case of 
	  failure, an error message will have been written to the error 
	  channel from inside CPLEX.  In this example, the setting of
	  the parameter CPXPARAM_ScreenOutput causes the error message to
	  appear on stdout.  */

	if ( net == NULL ) {
	  fprintf (stderr, "Failed to create network object.\n");
	  goto TERMINATE;
	}

	/* Fill in the data for the problem.  Note that since the space for
	  the data already exists in local variables, we pass the arrays
	  directly to the routine to fill in the data structures.  */

	double *supply;
	int* head;
	int* tail;
	double* obj;
	double* ub;
	double* lb;
	status = buildNetwork (env, net, this->widgetGraph, supply, head, tail, obj, ub, lb, opMode);

	// if(supply != NULL){
	// 	cout << "before not clear" << endl;
	// }

	if ( status ) {
	  fprintf (stderr, "Failed to build network problem.\n");
	  goto TERMINATE;
	}


	/* Optimize the problem and obtain solution. */

	status = CPXNETprimopt (env, net);
	if ( status ) {
	  fprintf (stderr, "Failed to optimize network.\n");
	  goto TERMINATE;
	}

	/* get network dimensions */

	narcs  = CPXNETgetnumarcs  (env, net);
	nnodes = CPXNETgetnumnodes (env, net);

	/* allocate memory for solution data */

	// x     = (double *) malloc (narcs  * sizeof (double));
	// dj    = (double *) malloc (narcs  * sizeof (double));
	// pi    = (double *) malloc (nnodes * sizeof (double));
	// slack = (double *) malloc (nnodes * sizeof (double));

	x     = new double[narcs];
	dj    = new double[narcs];
	pi    = new double[nnodes];
	slack = new double[nnodes];


	status = CPXNETsolution (env, net, &solstat, &objval, x, pi, slack, dj);
	// cout << "status: " << status << endl;
	if ( status ) {
	  // fprintf (stderr, "Failed to obtain solution.\n");
	  goto TERMINATE;
	}

	// cout << "results: " << endl;
	// for (int i = 0; i < narcs; ++i){
	//  cout << x[i] << endl;
	// }

	/* Write the output to the screen. */

	// printf ("\nSolution status = %d\n", solstat);
	if (solstat != 1 && solstat != 6 && solstat != 14){
		// cout << "status" << solstat << endl; 
		status1 = -1;
		goto TERMINATE;
	}
	// printf ("Solution value  = %f\n\n", objval);

	for (int k = 0; k < nnodes; ++k){
		for (int i = 0; i < this->widgetNodes[k]->edge_out.size(); ++i){
			this->widgetNodes[k]->edge_out[i].curr = x[cnt];
			WidgetNode* temp = this->widgetNodes[k]->edge_out[i].nodeTo;
			for (int j = 0; j < temp->edge_in.size(); ++j){
				if (temp->edge_in[j].nodeFrom == this->widgetNodes[k]){
					temp->edge_in[j].curr = x[cnt];
				}
			}
			cnt++;
		}
	}
	status1 = 0;

	// for (i = 0; i < nnodes; i++) {
	//   printf ("Node %2d:  Slack = %10f  Pi = %10f\n", i, slack[i], pi[i]);
	// }

	// for (j = 0; j < narcs; j++) {
	//   printf ("Arc  %2d:  Value = %10f  Reduced cost = %10f\n",
	//        j, x[j], dj[j]);
	// }

	/* Finally, write a copy of the problem to a file. */

	// status = CPXNETwriteprob (env, net, "netex1.net", NULL);
	// if ( status ) {
	//   fprintf (stderr, "Failed to write network to disk.\n");
	//   goto TERMINATE;
	// }
	
	
TERMINATE:

	delete [] supply;
	delete [] head;
	delete [] tail;
	delete [] obj;
	delete [] ub;
	delete [] lb;

	/* Free memory for solution data */

	delete [] x;
	delete [] dj;
	delete [] pi;
	delete [] slack;

	// free_and_null ((char **) &x);
	// free_and_null ((char **) &dj);
	// free_and_null ((char **) &pi);
	// free_and_null ((char **) &slack);

	/* Free up the problem as allocated by CPXNETcreateprob, if necessary */

	if ( net != NULL ) {
		// cout << "clearing net..." << endl;
	  status = CPXNETfreeprob (env, &net);
	 //  if (net != NULL){
		// cout << "fail to clear net" << endl;
	 //  }
	  if ( status ) {
		 fprintf (stderr, "CPXNETfreeprob failed, error code %d.\n", status);
	  }
	}

	/* Free up the CPLEX environment, if necessary */

	if ( env != NULL ) {
	  status = CPXcloseCPLEX (&env);
	 //  cout << "clearing env..." << endl;
	 //  if (env != NULL){
		// cout << "fail to clear env" << endl;
	 //  }
	//    Note that CPXcloseCPLEX produces no output,
	//   so the only way to see the cause of the error is to use
	//   CPXgeterrorstring.  For other CPLEX routines, the errors will
	//   be seen if the CPXPARAM_ScreenOutput indicator is set to CPX_ON. 

	// if(supply != NULL){
	// 	cout << "after not clear" << endl;
	// }

	  if ( status ) {
	  char  errmsg[CPXMESSAGEBUFSIZE];
		 fprintf (stderr, "Could not close CPLEX environment.\n");
		 CPXgeterrorstring (env, status, errmsg);
		 fprintf (stderr, "%s", errmsg);
	  }
	}

	return (status1);

}  /* END main */



static int
buildNetwork (CPXENVptr env, CPXNETptr net, WidgetGraph* widgetNet, 
	double * &supply, int* &head, int* &tail, double* &obj, double* &ub, double* &lb, int opMode)
{
	int status = 0;

	/* definitions to improve readability */

#  define inf    CPX_INFBOUND

	int nnodes = widgetNet->widgetNodes.size();
	int narcs = 0;
	for (int i = 0; i < nnodes; ++i){
		narcs += widgetNet->widgetNodes[i]->edge_out.size();
	}

	// cout << "num of nodes and arcs; " << nnodes << " " << narcs << endl;
	// supply = (double *) malloc (nnodes  * sizeof (double));
	// tail = (int *) malloc (narcs  * sizeof (int));
	// head = (int *) malloc (narcs  * sizeof (int));
	// obj = (double *) malloc (narcs  * sizeof (double));
	// ub = (double *) malloc (narcs  * sizeof (double));
	// lb = (double *) malloc (narcs  * sizeof (double));

	supply = new double [nnodes];
	tail = new int [narcs];
	head = new int [narcs];
	obj = new double [narcs];
	ub = new double [narcs];
	lb = new double [narcs];

	// initialize supply and demand
	for (int i = 0; i < nnodes; i++){
		if (widgetNet->widgetNodes[i]->type == SUPER_SOURCE){
			supply[i] = 1 * widgetNet->payment;
		} else if (widgetNet->widgetNodes[i]->type == SUPER_DEST){
			supply[i] = -1 * widgetNet->payment;
		} else {
			supply[i] = 0;
		}
	}

	// initialize LP with widget net
	int cnt = 0;

	for (int k = 0; k < nnodes; ++k){
		for (int i = 0; i < widgetNet->widgetNodes[k]->edge_out.size(); ++i){
			WidgetOutEdge temp = widgetNet->widgetNodes[k]->edge_out[i];
			head[cnt] = widgetNet->widgetNodes[k]->nodeID;
			tail[cnt] = temp.nodeTo->nodeID;
			// obj[cnt] = temp.interest_diff;
			// cout <<opMode<<endl;
            if (opMode == 1) {
                if (widgetNet->widgetNodes[k]->originNode == widgetNet->src
                    && temp.nodeTo->type == 2){
                    obj[cnt] = temp.interest_diff;
                } else {
                    obj[cnt] = 0;
                }
            }
			else if(opMode==2) {
				if (widgetNet->widgetNodes[k]->originNode == widgetNet->src
                    && temp.nodeTo->type == 2){
                    obj[cnt] = temp.interest_diff;
                }
				else if (widgetNet->widgetNodes[k]->originNode == widgetNet->dest
                    && widgetNet->widgetNodes[k]->type == 3) {
					obj[cnt] = -temp.interest_diff;
				}
				else {
                    obj[cnt] = 0;
                }
			}
			else if(opMode==3) {
				if (widgetNet->widgetNodes[k]->originNode == widgetNet->src
                    && temp.nodeTo->type == 2){
                    obj[cnt] = -temp.interest_diff;
                }
				else if (widgetNet->widgetNodes[k]->originNode == widgetNet->dest
                    && widgetNet->widgetNodes[k]->type == 3) {
					obj[cnt] = temp.interest_diff;
				}
				else {
                    obj[cnt] = 0;
                }
			}			
			else if(opMode==4) {
				obj[cnt] = -1;
			}
			else {
                obj[cnt] = temp.interest_diff;
            }
            

			ub[cnt] = temp.cap;
			lb[cnt] = 0;
			// cout << "cnt: " << cnt << endl;
			cnt++;
		}
	}

	// cout << "node ids: "; 
	// for (int i = 0; i < widgetNet->widgetNodes.size(); ++i){
	//  cout << widgetNet->widgetNodes[i]->nodeID << " ";
	// }
	// cout << endl;
	// cout << "supply: ";
	// for (int i = 0; i < nnodes; ++i){
	//  cout << supply[i] << " ";
	// }
	// cout << endl;
	// cout << "arc: " << endl;
	// for (int i = 0; i < narcs; ++i){
	//  cout << head[i] << " " << tail[i] << ", " 
	//      << obj[i] << " " << ub[i] << " " << lb[i] << endl;
	// }

	if ( CPXNETgetnumnodes (env, net) > 0 ) {
	  status = CPXNETdelnodes (env, net, 0,
								CPXNETgetnumnodes (env, net)-1);
	  if ( status ) goto TERMINATE;
	}

	/* Set optimization sense */

	status = CPXNETchgobjsen (env, net, CPX_MAX);
	if ( status ) goto TERMINATE;

	/* Add nodes to network along with their supply values,
	  but without any names. */

	status = CPXNETaddnodes (env, net, nnodes, supply, NULL);
	if ( status ) goto TERMINATE;

	/* Add arcs to network along with their objective values and
	  bounds, but without any names. */

	status = CPXNETaddarcs (env, net, narcs, tail, head, lb, ub, obj, NULL);
	if ( status ) goto TERMINATE;

	// if (supply != NULL){
	// 	cout << "before exit build, supply not empty" << endl;
	// }

TERMINATE:

	return (status);

}  /* END buildnetwork */


static void
free_and_null (char **ptr)
{
	if ( *ptr != NULL ) {
	  free (*ptr);
	  *ptr = NULL;
	}
} /* END free_and_null */