// File: stats.h 
// stats.h is the interface for a simple statistics module.
// @author bks: ben k steele
// @author Erikson Sodergren
// @date 02/02/2016
// 
// git repository: ~{/home/stu11/s8/exs1084/Courses/CS243/HW2}
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// TODO ADD INCLUDE GUARDS TO THIS FILE


/// TODO write a full docstring for average, including @params and @return

double average( int count, const int scores[] ) ;

/// TODO write a full docstring for std_dev

double std_dev( int count, const int scores[], double mean ) ;

/// TODO write a full docstring for histogram

void histogram( int count, const int scores[] ) ;

