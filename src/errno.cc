namespace iRRAM {

#define ERRORDEFINE(x,y) y,
const char* iRRAM_error_msg[]={
#include <iRRAM/errno.h>
};

}
