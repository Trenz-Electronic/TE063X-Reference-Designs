Copyright (C) 2012 Trenz Electronic

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.
--------------------------------------------------------------------------------
Reference project for TE0630 boards.

To work with software part:

- Launch Xilinx SDK and specify new workspace.

- In Xilinx SDK "Xilinx Tools"->"Repositories". Add new repository which point
  to ..\TE-EDK-IP
  
- Import standalone_bsp_0 and demo from sw folder to your workspace.

- Build project.

- Launch Xilinx EDK and do right mouse click to "Project files"->"Elf files"->
  "microblaze_0"->"Imp Executable" select Browse and specify demo.elf file in
  your workspace
  
- Do "Device Configuration"->"Update Bitstream"

Project is under MIT license. All contributions should mit MIT license.
--------------------------------------------------------------------------------
