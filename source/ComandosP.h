char PKG_I = 127;//valor de inicio del paqeute de datos
char PKG_F = 100;//valor de final del paqeute de datos

#define errNone             0
#define errCommunic         1
#define errCheckSum         2
#define errSendGraph        3

#define ctempA              62
#define cq1A                63
#define cq2A                64
#define cgetAllData         65
#define cHourA              66
#define cgetDesiredState    67
#define cgetTempPID         68
#define cgetQ1PID           69
#define cgetQ2PID           70
#define cgetVol             71
#define cgetPass            72
#define cgetOnOff           73
#define ctemp               74
#define cq1                 75
#define cq2                 76
#define ctempkP             77
#define ctempkI             78
#define ctempkD             79
#define cq1kP               80
#define cq1kI               81
#define cq1kD               82
#define cq2kP               83
#define cq2kI               84
#define cq2kD               85
#define cOnOff              86
#define cPass               87
#define cError              88
#define cChecking           89
#define cSummarySendInit    90 //also send the startHour of the process
#define cSummarySendEnd     91 //also send the endHour of the process
#define cSummarySendItem    92 //a float number for a point
#define cSummarySendDetails 93 //n, n1, err1:err0 (error codes if any)