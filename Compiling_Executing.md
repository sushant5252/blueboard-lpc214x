**Blueboard Compiling and Executing Code- First Timer’s Manual**

**a.Rowley Crossworks.**

i. After downloading the source code, open the folder named ‘Crossworks’, in that        open file named ‘Blueboard’.

ii. A ‘Project Explorer’ window opens.

iii. Click on ‘Build’ and then ‘Build Blueboard’.

iv. The output window at the bottom of ‘Project Explorer’ will display message ‘Build Complete’ without any errors and warnings.

v. A folder ‘Build’ is created in the ‘Crossworks’ folder which contains the ‘hex’ file.


**b.GNU**

i. After downloading the source code, download ‘utils.zip’ from here: (add link) and extract the files.

ii. From the extracted file copy the contents of the folder named ‘bin’ and paste them here ‘C:\Program Files\GNUARM\bin’.

iii. Now open command prompt and locate the path of the folder named ‘GCC’ which is in the folder where you have downloaded the source code.

iv. Type ‘make’ and press ENTER. This will create a hex file in the OBJ directory.

**c.WINARM**

i.	Install WinARM from here :
http://winarm.scienceprog.com/winarm-tools/quick-start-using-winarm.html
Set your environment variables to
PATH c:\WinARM\bin;c:\WinARM\utils\bin; (assuming you have extracted WinARM directory structure into C:\WinARM)

ii. Open ‘Programmers Notepad 2’ from the WinARM folder which is located at:C:\WinARM\pn

iii. In the ‘Programmers Notepad 2’ window Click on File >New>Project.
A ‘New Project…’ window opens. Enter the project name and the location to store it.

iv. The new project created is shown on the left side in projects window.
Right Click on the project and select ‘Add Magic Folder’. Locate the path of ‘Source’ folder present in the folder where you downloaded the source code and click ‘Next’.

v. Click ‘Option’ and select ‘Make all’. This will create hex will in the folder where your project is created.