#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries.
 *  This example demonstrates using RPC over serial
**/
RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled3(LED3,"myled3");
Serial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
RPCFunction rpcLED(&LEDControl, "LEDControl");
double x, y;

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class
    
    // receive commands, and send back the responses
    char buf[256], outbuf[256];
    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = pc.getc();
            if (recv == '\r') {
                pc.printf("\r\n");
                break;
            }

            buf[i] = pc.putc(recv);
        }

        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        pc.printf("%s\r\n", outbuf);
    }
}

// Make sure the method takes in Arguments and Reply objects.
void LEDControl (Arguments *in, Reply *out)   {
    bool success = true;
    myled1.write(1);
    myled3.write(1);

    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();

    // Have code here to call another RPC function to wake up specific led or close it.
    char buffer[200], outbuf[256], buffer1[200], outbuf1[256];
    char st1[20], st2[20];
    int led1 = x;
    int n = sprintf(st1, "/myled%d/write 0", led1);
    strcpy(buffer, st1);
    RPC::call(buffer, outbuf);
    if (success) {
        out->putData(buffer);
    } else {
        out->putData("Failed to execute LED control.");
    }
    wait(1);
    int n1 = sprintf(st2, "/myled%d/write 1", led1);
    strcpy(buffer1, st2);
    RPC::call(buffer1, outbuf1);
    if (success) {
        out->putData(buffer1);
    } else {
        out->putData("Failed to execute LED control.");
    }
    wait(1);
}