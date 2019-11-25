
/*
* Dated : 05/August/2014 , 16:43PM
* Author  Amber N Pachauri
* Purpose: This is TurboC 3.0/ DOS program utility created with concept
*  of Virtual-Machine, and it runs based on commands being passed
*  with each command instruction is in itself encrypted data
*  for which gets decoded and stored and read back again.
*
*  The VM is like dynamic stack which keeps un/rolling with the
*  kind of data being read from sources of kind : *.bin fles.
*
*  These files are provided from within the code respository.
*  Files are like task1.bin; task2.bin and list of test cases files
*  testxx.bin
*
*  For better understandng have used variales, functions and
*  code layout almost equivalent as speciied in task1.txt.
*
*  On executing this module two files are created directories
*  - task1/result.txt
*  _ task2/result.txt
*/
#include<stdio.h>
#include<stdlib.h>


/*-----------------------------------------------
* some gobal declarations/definations , which are used
* across functions and VM runtime environment
*------------------------------------------------*/
long int ip, sp, sptr, *pCurrInstr;

//variable to hold address locations.
long int **pData = NULL;
FILE *pTask, *pTask1, *pTask2;  // file pointer

//variable to hold, extracted decoded values
//in form of operation, Optional data and Binary operation.
long int iOper = 0x00;
long int iOpt = 0x00;
long int iBinop;

//variable to indicate STOP running VM.
int iVMStop;


#define VM_FILE_PATH "e:\\task1.bin"
//task2.bin

//#define VM_TEST_PATH "e:\\test25.bin"
//#define VM_TEST_PATH "e:\\test25.bin"
//some test files test01.bin...test25.bin


#define VM_FILE_TASK1 "e:\\task1\\result.txt"
#define VM_FILE_TASK2 "e:\\task2\\result.txt"

// some forward declarations
void perform_VM(int iBinoper , int iOperation);
//execute VM operations.
void decode_VM(long int *pDataCode);
//decode instruction in VM files.

/*-----------------------------------------------
* some gobal definations , which are used
* across VM runtime environment
*------------------------------------------------*/

void f(long int iOptionalData)
{ // routine to allow data to be stored
  // and stack pointer is decremented

  sp = sp -1;
  fprintf(pTask2,"storing-%ld at loc:%d\n", iOptionalData, sp);
  *pData[sp] = iOptionalData;

}


long int g(void)
{ // routine which allows to read back data
  // and send to stdout when requested.

  long int iTmp = *pData[sp];
  sp = sp + 1;
  fprintf(pTask2,"return data-%ld\n", iTmp);

  return iTmp;
}


/* This module is used to decode the
 * data values and further perform operations,
 * assupplied from data-array, while reading insruction
 *
 * Parameters:
 *  pDataCode  - holds the encrypted data.
 *
 * Bits are aligned from MSB-LSB ; left->right 31..0;
 * bits position :
 * position 31   : binop , 1-bit.
 * positin 30-23 : operation, 7-bits.
 * position 23-0 : optional data, 24-bits
 */
void decode_VM(long int *pDataCode)
{

 //exract binop
 long int iMask = 0x01;

 long int iCheck, iValue;
 long int iTmp = 0x00;


  //int j, i =0, j = 0;

  iBinop = iOpt = iOper = 0x00;

  iValue = iCheck = *pDataCode;

  fprintf(pTask2,"received instr(%ld) \nDecoding..", *pDataCode);
  iMask |= 1 << 31;
  iValue = iValue & iMask;

  iBinop &= iValue;
  fprintf(pTask2,"binop-%d ;\t", iBinop);

  //extract operation
  iValue |= *pDataCode;
  //iValue |= iCheck;
  //printf("val1 =%ld; chk1-%ld", iValue, iCheck);
  iTmp |= (iValue >> 24);
  //printf("\ntmp =%ld; chk3-%ld", iTmp, iCheck);
  iTmp &= 0xfff;  //set those particular pattern

  iOper |= iTmp;
  fprintf(pTask2,"oper-%d ;\t", iOper);
   //getchar();

  //extract the optiona bits
  iOpt |= *pDataCode;
  //printf("data-%ld..\t", iOpt);
  iOpt &= 0xfff;  //set the lower 24 bits;
  fprintf(pTask2,"optional-%d.\n", iOpt);

}



/* This routine is used to perform operations over the
 * contents in dynamic stack, based on the few arguments
 * extracted before - operations, optional data, and Binop
 *
 * Parameters:
 *   iBinoper - '1' or '0' as specified.
 *   iOperation  - 0..10 as specified.
 */
void perform_VM(int iBinoper , int iOperation)
{

  long int addr, st_data;
  long int res, a, b, tData;
  char oneByte;
  long int cond, x32Bits;


    switch(iOperation)
      {
       // operations states for VM to operate with.

      /* Note : there is problem or some of the case
       * operation bit values is not defined ?? or may be
       * code implemented below is bit incorrect; for example
       * case bit value for 127, 15 etc.
       * please re-check?
       * if its signed bit should i carry over and assume
       * 127 as 0.
       */
	  //case 127:
	  case 0:
	  {
	     if (iBinoper){///0=> add, <op> is +
		fprintf(pTask2,"0=> add, <op> is +\n");
		b = g();
		a = g();
		res &= (a+b);
		f(res);


	     }else{  //pop sp= sp+1;
		fprintf(pTask2,"incr 'sp'\n");
		sp = sp +1;
	     }
	  }break;

	  //case 127:
	  case 1:
	  {
	    if ( iBinoper) { /// 1= sub, <op> is -
		fprintf(pTask2,"1=> sub, <op> is -\n");
		b = g();
		a = g();
		res &= (a-b);
		f(res);

	    }else {  //push const(optinal)
	      fprintf(pTask2,"push 'constant'\n");
	      f(iOpt);
	    }
	   }break;

	   case 2:
	   {
	     if (iBinoper) {//2= mul, <op> is *
	       fprintf(pTask2,"2=> mul, <op> is *\n");
	       b = g();
	       a = g();
	       res &= (a*b);
	       f (res);

	     }else { //push ip
	       fprintf(pTask2,"push 'ip'\n");
	       f(ip);
	     }
	   }break;

	   case 3:
	   {
	     if (iBinoper) { ///3-div,<op> is /
	       fprintf(pTask2,"3=>div,<op> is / \n");
	       b = g();
	       a = g();
	       res &= (a/b);
	       f (res);

	     } else { //push sp
	       fprintf(pTask2,"push 'sp' \n");
	       f(sp);
	     }
	   }break;

	   case 4: //load
	   {
	     if (iBinoper){ ///4-and, <op> is &
	       fprintf(pTask2,"4=> and, <op> is & \n");
	       b = g();
	       a = g();
	       res &= (a&b);
	       f (res);

	     }else {

	       fprintf(pTask2,"load addr\n");
	       addr = g();
	       f(*pData[addr]);
	     }
	   }break;

	   case 5: //store
	   {
	     if (iBinoper) { ///5- or ,<op> is |
	       fprintf(pTask2,"5=> or ,<op> is |\n");
	       b = g();
	       a = g();
	       res &= (a|b);
	       f (res);

	     } else {
	       fprintf(pTask2,"store addr\n");
	       st_data = g();
	       addr = g();
	       *pData[addr] = st_data;
	     }

	   }break;

	   case 6: //jmp
	   {
	     if (iBinoper) { ///6- xor ,<op> is ^
	       fprintf(pTask2,"6=> xor ,<op> is ^\n");
	       b = g();
	       a = g();
	       res &= (a^b);
	       f(res);

	     }else {
	       fprintf(pTask2,"implement setjmp\n");
	       cond = g();
	       addr = g();

	       if (0 != cond) {
		 ip = addr;
	       }
	     }
	   }break;

	   case 7:
	   {
	     if (iBinoper) { //7- eq ,<op> is =
	       fprintf(pTask2,"7 => eq ,<op> is = \n");
	       b = g();
	       a = g();

	       res = ((a==b) ? 1 : 0);
	       f(res);

	     } else {  //not
	       fprintf(pTask2," not condition\n");

	       if ( !g() )
		 f(1);
	       else
		 f(0);
	     }
	   }break;

	   case 127:
	  /* note : often the operational bit '8'
	   * in which case should be binary map
	   * msb->lsb : 10000000 can be assumed as same??
	   * More precisely there is no action specified
	   * for task1.txt for such case - should assume
	   * carry-over bit?
	   * So either to make it more sensibe for me it could
	   * be carried over bit? in which case i should call
	   * case '0' or lets see and check out if we print
	   * charcters to stdout??*/
	   case 8: //putc
	   {
	     if (iBinoper) {
	      //8- lt ,<op> is <
	       b = g();
	       a = g();

	       res = ((a<b) ? 1 : 0);
	       f(res);

	      }else {
	       //output exactly one-byte to
	       //stdout, ASCII text

	       tData = g();
	       oneByte = (char) (tData&0xff);

	       //printf("byte2stdout: %c\n", oneByte);
	       //putc(oneByte,stdout);
	       fprintf(pTask2,"byte2stdout: %c\n" ,oneByte);
	      }
	    }break;

	    case 9:
	    { //read exactly one byte from
	      //stdin and cast to 32bts
	       printf("enter onebyte:\n");
	       getchar();
	       oneByte = getc(stdin);
	       x32Bits = (long int)oneByte;
	       f( x32Bits & 0xff);

	    }break;

	    case 10: //halt
	    {//0x0A
	      fprintf(pTask2,"\nVirtual-Machine, halting.\n");
	      iVMStop = 1;
	    }break;

	   //no default case;
	   default:
	     fprintf(pTask2,"operation(%d) not defined..\n",iOperation);
      }
}


/* The VM main entry point*/
int main(int argc , char* argv[])
{

	int i, iDataIndex =0;
	int iGetDataSz, iGetImageSz;
	// flags to check inernal state machine.

	long int iDataSz;   //variabe for data-size
	long int iImageSz;  //variable for image-size

	char ch, cData[10];
	//variabe to reading each lines , character-by-character

	iImageSz = iDataSz = iGetDataSz = iGetImageSz =0;

	pTask = pTask1 = pTask2 = NULL;

	pTask = fopen(VM_FILE_PATH, "r");

	//pTask = fopen(VM_TEST_PATH, "r");

	//pTask = fopen(VM_FILE1_PATH, "r");
	// open the output file
	
	pTask1 = fopen(VM_FILE_TASK1, "w+");
	if ( !pTask1 ){
	   printf("No output task-1 file, exiting");
	   exit(0);
	}


	//check if file exists?
	if ( pTask != NULL) {
	   fprintf(pTask1,"VM opened, successfuly.\n");
	   i =0;
	   cData[i] = 0;

	   printf("\n VM & output files opened.");
	   printf("\n Running VM tasks...");
	   while ( !feof (pTask) ) // read whole file
	   {
	       ch = fgetc(pTask);
	       if ( ch == '\n'){  //read each line
		   cData[i] = 0;
		   fprintf(pTask1, "VM-entry: %s\n", cData);
		   //getchar();
		   cData[i] = 0;
		   if( !iGetDataSz ) {
		      //convert to datasize
		      iGetDataSz =1;

		      iDataSz = strtol(cData, 0, 16);
		      fprintf(pTask1, "VM data-size: %ld\n", iDataSz);

		      //allocate fordata-size elemsts
		      pData = (long int**) malloc (sizeof(long int*) * (iDataSz) );

		      for ( i =0; i< iDataSz; i++){
		      //allocate all pointers to array index length - 32bits.
			  pData[i] = (long int*) malloc ( sizeof (long int));
		      }
		      i=0;

		      continue;
		   }//data-size

		   if( !iGetImageSz ) {
		      //convert to imagesize
		      iGetImageSz =1;
		      iImageSz = strtol(cData, 0, 16);
		      fprintf(pTask1,"VM Image-size: %ld\n", iImageSz);

		      i=0;
		      continue;
		   }//image-size

		   //load VM
		   if(( pData != NULL) && (iGetImageSz)){
		      fprintf(pTask1,"VM data: %ld \n", strtol(cData, 0 ,16));
		      *pData[iDataIndex++] = strtol(cData, 0, 16);
		      i =0;
		      continue;
		   }
	       }//reached newline check??

	       cData[i++] = ch;  //keep reading char

	   }// finished loading VM

	   fprintf(pTask1,"\nVM stack-enteries-%d", iDataIndex);
	   printf("\n finished loading VM .");
	   /* Safe to close VM file - VM data is read
	    * and loaded into memory. */
	   fclose(pTask);
	   fclose(pTask1);

	   pTask1=pTask = NULL;

	   pTask2 = fopen(VM_FILE_TASK2, "w+");
	   if ( !pTask2 ){
	      printf("No output task2-file, exiting");
	      exit(0);
	   }

	   //continue...to Interpret VM images
	   ip = 0;
	   //sptr = iDataIndex;
	   sp = iDataSz;

	   iVMStop = 0;
	   printf("\n Executing VM tasks...");
	   do {
		//execution of VM

	       pCurrInstr = pData[ip++];
	       //ip++;
	       fprintf(pTask2,"\n\nSending-%ld ;\t", *pCurrInstr);
	       printf("\nReading memory: %ld", *pCurrInstr);

	       decode_VM(pCurrInstr);
	       //pass the instruction, decode the bits
	       perform_VM(iBinop, iOper);
	       //perform VM related, operations.

	       if (iVMStop)
		 break;

	  }while(1);

	  printf(" \n VM halted.");
	  fclose(pTask2);
	  free(pData);
	}
	else
	   printf("file NOT exists..\n");

       //getchar();
       //stop console from exiting!!

  return 0;
}
