Assembly language และ assembler

        ส่วนแรกของ project คือการอ่าน assembly language และแปลงให้เป็น machine code ยกตัวอย่างเช่น คำสั่ง beq ก็จะถูกแปลงไปเป็น เลข 100 (opcode) และส่วนที่เป็น ชื่อ symbolic ก็จะถูกแปลงไปเป็น ตัวเลข และผลสุดท้ายจะได้ 32-bit instructions (ใน project นี้ bits ที่ 31-25 จะถูก ตั้งไว้ที่ 0)  Format ของการเขียน assembly code แสดงข้างล่าง (หมายเหตุ <white> คือ tabs และ/หรือ ช่องว่าง)

label<white>instruction<white>field0<white>field1<white>field2<white>comments

 

ส่วนซ้ายสุดคือ label field ซึ่งจะมีได้มากที่สุด 6 ตัวอักษรและประกอบด้วยตัวหนังสือและตัวเลขแต่จะต้องเริ่มด้วยตัวหนังสือ โดยปกติจะมีหรือไม่มี Label ก็ได้ หลังจากนั้นก็เป็น white space ซึ่งตามด้วย instruction field ซึ่งจะเป็น instruction ที่อยู่ใน table 1 หลังจากนั้นจะเป็น white space และ fleids ซึ่ง field เหล่านี้จะเป็น ตัวเลข decimal หรือ label จำนวนของ fields จะขึ้นอยู่กับ ชนิดของ instruction ส่วน filed ที่ไม่ถูกใช้ให้คิดว่าเป็น comment

        R-type instructions (add, nand) มี 3 fields (field0 คือ regA, field1 คือ regB, field2 คือ destReg)

        I-type instructions (lw, sw, beq) มี 3 fields (field0 คือ regA, field1 คือ regB, field2 เป็น ค่าตัวเลขสำหรับ offsetField ซึ่งเป็นได้ทั้ง บวกหรือลบ หรือ symbolic address ซึ่งจะกล่าวถึงข้างล่าง

        J-type instructions (jalr) มี 2 fields (field0 คือ regA, field1 คือ regB)

        O-type instruction (noop, halt) ไม่มี field

Symbolic address คือ label และassembler จะคำนวณ offsetField โดยให้เท่ากับ address ของ label สำหรับ lw, sw instructions การทำเช่นนี้จะทำได้โดยใช้ zero-base register ในการอ้างถึง label หรือ ใช้ non zero-base register ในการชี้ไปที่ array ที่เริ่มที่ label ส่วน beq instruction assembler จะแปลง label ให้เป็นตัวเลข offsetField เพื่อบอกถึงที่อยู่ที่จะกระโดดไปที่ label นั้น

ส่วนที่เป็น comment จะมี end of line บอกว่า หมดส่วนนี้แล้ว

        อีกคำสั่งที่ควรจะมีคือ .fill คำสั่งนี้จะบอก assembler ให้ใส่ตัวเลขในที่ที่ instruction ควรจะอยู่ และคำสั่งนี้ใช้ 1 field นั่นคือ ตัวเลขหรือ symbolic address คัวอย่างเช่น ”.fill 32” คือการที่เอาเลข 32 ไปไว้ที่ที่ instruction ควรจะอยู่ ส่วน .fill ที่ใช้ symbolic address จะเก็บค่า address ของ label เช่นตัวอย่างข้างล่าง “.fill start” จะเก็บ ค่า 2 เพราะ label “start” อยู่ที่ address 2

        Assembler ควรจะคำนวณทุก symbolic address ก่อนและสมมุติว่า instruction แรก อยู่ที่ address 0 และมันจะสร้าง machine code (เลขฐาน 10) สำหรับทุกบรรทัดใน assembly language ตัวอย่างเช่น assembly language program ที่ นับเลขจาก 5 จนถึง 0

          lw       0        1        five      load reg1 with 5 (uses symbolic address)

          lw       1        2        3        load reg2 with -1 (uses numeric address)

start    add     1        2        1        decrement reg1

          beq     0        1        2        goto end of program when reg1==0

          beq     0        0        start    go back to the beginning of the loop

          noop

done    halt                                  end of program

five      .fill      5

neg1    .fill      -1

stAddr  .fill      start                        will contain the address of start (2)

 

และนี้คือ machine language ที่ถูกแปลงมาจาก assembly ข้างบน:

 

(address 0): 8454151 (hex 0x810007)

(address 1): 9043971 (hex 0x8a0003)

(address 2): 655361 (hex 0xa0001)

(address 3): 16842754 (hex 0x1010002)

(address 4): 16842749 (hex 0x100fffd)

(address 5): 29360128 (hex 0x1c00000)

(address 6): 25165824 (hex 0x1800000)

(address 7): 5 (hex 0x5)

(address 8): -1 (hex 0xffffffff)

(address 9): 2 (hex 0x2)

เนื่องจาก program จะเริ่มที่ 0 เสมอดังนั้นควรจะ แสดงผล ที่ไม่ใช่ address นั่นคือ

8454151

9043971

655361

16842754

16842749

29360128

25165824

5

-1

2

        Assembler ที่เขียนขึ้นนี้ควรจะมีส่วนที่จะหา errors ที่จะกล่าวถึงต่อไปนี้

          1. การใช้ labels ที่ undefine

          2. การใช้ label ที่เหมือนกัน

          3. การใช้ offsetField ที่มีจำนวน bit มากกว่า 16 bits

          4. การใช้ opcode นอกเหนือจากที่กำหนด

        Assembler ควรจะใช้ exit(1) ถ้ามี error และ exit(0) ถ้า program สามารถทำงานได้โดยที่ไม่เจอ error แต่เนื่องจากนี้ไม่ใช่ simulator ดังนั้น assembler ไม่ควรจะ detect error ที่จะเกิดในขั้นตอน ของการ simulation เช่น infinite loop หรือ กระโดดไปที่ address –1 ฯลฯ

        ในการทดสอบ assembler จงใช้  assembly program ข้างต้น และ assembly program อย่างอี่น อีกเพื่อเป็นการทดสอบ assembler และอย่าลืมทดสอบ การ check error  ของ assembler ด้วย

          อย่าลืมว่า offsetField ใช้ 2’s complement ดังนั้นมันสามารถเก็บเลขได้ตั้งแต่ –32768 ถึง 32767 และสำหรับ symbolic addresses assembler จะทำการคำนวณ offsetField เพื่อที่จะอ้างถึง label ที่ถูกต้อง อย่าลืม check ว่า เครื่องที่ใช้ เป็น integer 32 หรือ 16 bits ถ้าเป็น 32 bits ต้องตัดเอาเฉพาะ 16 bits negative values ของ offsetField เพราะ assembler ที่สร้างใช้ 16 bits 2’s complement number สำหรับ offsetField# SMC-Assembler
Assembly language และ assembler

