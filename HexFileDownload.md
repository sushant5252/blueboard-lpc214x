**How to download ‘HEX’ file in Blueboard**


Download ‘Flash Magic’ from here: http://www.flashmagictool.com/
We will use this utility to burn our hex file into Blueboard.

1.Open the flash magic tool and configure the com port settings as below:

| **Parameter** | **Permissible values** | |
|:--------------|:-----------------------|:|
|COM Port       |Select COM Port         | |
|Baud           |9600 to 38400           | |
|Device         |LPC 2148                | |
|Interface      |NONE (ISP)              | |
|Oscillator     |12.000000               | |


Only the above mentioned configurations have been tested.

2.Choose the ‘hex’ file generated.

3.Choose ‘Erase all flash+ Code Rd Prot’ and ‘verify after programming’ and ‘Fill unused Flash’ options before programming.

4.Press ‘START’.

5.If you are using full serial cable due to hardware limitation on Blueboard, to execute the downloaded program you need to,

a.Power OFF the board.

b.Remove Serial Cable

c.Power the board back ON.

Now you should be able to see your program executing.

6.However, if you are using a half serial cable, to execute the downloaded program you need to press the RESET switch (SW3) or you can turn OFF and ON your board.