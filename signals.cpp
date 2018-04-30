// signals.cpp
// contains signal handler functions
// contains the function/s that set the signal handlers (catch_ctrl_z, catch_ctrl_c)

#include "signals.h"

//**************************************************************************************
// function name: catch_ctrl_z
// Description: Ctrl-Z signal handler
// Parameters: None
// Returns: void
//**************************************************************************************
void catch_ctrl_z(int sig_id){
	stop_job(); // forwards the command handle to “commands” how manage the jobs
}


//**************************************************************************************
// function name: catch_ctrl_z
// Description: Ctrl-Z signal handler
// Parameters: None
// Returns: void
//**************************************************************************************
void catch_ctrl_c(int sig_id){
	kill_job(); // forwards the command handle to “commands” how manage the jobs
}

//**************************************************************************************
// function name: send_signal
// Description: end the signal “sig_id” to process PID and print a matching massage to the user.
// Parameters: pid: the PID of the process; sig_id: the signal the send.
// Returns: (int) 0 - success, 1 - failure.
//**************************************************************************************
int send_signal(pid_t pid, int sig_id){
	cout << "signal " << get_sig_name(sig_id) << " was sent to pid " << pid << endl; // print a massage even if the signal failed
	return kill(pid, sig_id);
}

//**************************************************************************************
// function name: get the signal name (string) by its id.
// Description: get the signal name (string) by its id.
// Parameters: sig_id: the signal id.
// Returns: (string) signal name.
//**************************************************************************************
string get_sig_name(int sig_id)
{

	switch (sig_id)
	{
		case 1: return "SIGHUP";
		case 2: return "SIGINT";
		case 3: return "SIGQUIT";
		case 4: return "SIGILL";
		case 5: return "SIGTRAP";
		case 6: return "SIGABRT or SIGIOT";
		case 7: return "SIGBUS";
		case 8: return "SIGFPE";
		case 9: return "SIGKILL";
		case 10: return "SIGUSR1";
		case 11: return "SIGSEGV";
		case 12: return "SIGUSR2";
		case 13: return "SIGPIPE";
		case 14: return "SIGALRM";
		case 15: return "SIGTERM";
		case 16: return "SIGSTKFLT";
		case 17: return "SIGCHLD";
		case 18: return "SIGCONT";
		case 19: return "SIGSTOP";
		case 20: return "SIGTSTP";
		case 21: return "SIGTTIN";
		case 22: return "SIGTTOU";
		case 23: return "SIGURG";
		case 24: return "SIGXCPU";
		case 25: return "SIGXFSZ";
		case 26: return "SIGVTALRM";
		case 27: return "SIGPROF";
		case 28: return "SIGWINCH";
		case 29: return "SIGIO";
		case 30: return "SIGPWR";
		case 31: return "SIGSYS";
	default:
		return "no signal matching this id";
	}
}






