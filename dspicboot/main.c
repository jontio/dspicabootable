
#include <main.h>

//uses lots of pages for debugging
#if defined(DEBUG) || defined(SIMULATOR)
#include <stdlib.h>
#define BOOT_LOADER_START 0x00014000
#define ORG_GOTO_INSTRUCTION_ADDRESS 0x00013FFC //BOOT_LOADER_START-4 pcwhd doesn't honnor this kind of thing for defines
#else
//uses one page and a bit
#define BOOT_LOADER_START 0x00015400
#define ORG_GOTO_INSTRUCTION_ADDRESS 0x000153FC //BOOT_LOADER_START-4 pcwhd doesn't honnor this kind of thing for defines
#endif

//#define BOOT_LOADER_START 0x00014000
//#define ORG_GOTO_INSTRUCTION_ADDRESS 0x00013FFC

//room for uploaded program
#ORG 0x000200, BOOT_LOADER_START-2-4 {} //this is the last usable address

#define BOOTLOADER_PAGE (BOOT_LOADER_START>>10)
#define ORG_RESET_INSTRUCTION_PAGE (BOOT_LOADER_START>>10)-1
#define HASH_ORG_RESET_INSTRUCTION_PAGE #ORG_RESET_INSTRUCTION_PAGE

#ORG ORG_GOTO_INSTRUCTION_ADDRESS //take 4 so the goto is not on the same page as we are in
rom uint8 org_reset_instruction[8]={00,00,04,00,00,00,00,00};//goto 0;


#ifdef SIMULATOR
//data to bootload if we are simulating
//rom char prog_text[256]= {":080000002A03040000000000C7:100650000028EF002A03040000000000140020001E:00000001FF!"};
//rom char prog_text[800]= {":080000001802040000000000DA:100400000000E0004220AF00080037003B1C09005C:1004100000000000FE3F090000000000FE3F090050:10042000000000000000E900F8FF3A0000000600AC:100430000FF82300F0FF230020A0B7000000000009:1004400081E0A8000600FC00217420006204200066:1004500073052000824878008348780042C7A900CD:1004600090F02300A4A6B70030002000C4A6B70077:100470000600FC00217420006204200073052000A7:10048000824878008348780042C7A800040028000A:100490000411880004402000141188002062A80084:1004A00064052000441188002CA3EF000028EF0011:1004B000C442A800C462A800401F2000000202003D:1004C00000000000C462A900401F200000020200DA:1004D00000000000C462A8005C02040000000000EC:0404E0000040FE00DA:0200000401F009:10000000CF0000FFCF0000FF070000FF810000FFCE:10001000230000FF400000FF170000FFC30000FFA7:00000001FF!"};
//rom char prog_text[800]= {":080000001A12040001000000C7:020000040002F8:0C2404000000E0004220AF00080037009C:102410003B1C090000000000FE3F09000000000016:10242000FE3F0900000000000000E900F8FF3A004C:10243000000006000FF82300F0FF230020A0B700E3:102440000000000081E0A8000600FC0021742000CC:1024500062042000730520008248780083487800D9:1024600042C7A90090F02300A4A6B70030002000C6:10247000C4A6B7000600FC002174200062042000FE:1024800073052000824878008348780042C7A8007E:102490000400280004118800044020001411880062:1024A0002062A80064052000441188002CA3EF00DE:1024B0000028EF00C442A800C462A800401F20000A:1024C0000212020001000000C462A900401F2000A7:1024D0000212020001000000C462A8005E120400A3:0824E000010000000040FE00B5:0200000401F009:10000000CF0000FFCF0000FF070000FF810000FFCE:10001000230000FF400000FF170000FFC30000FFA7:00000001FF!"};
//rom char prog_text[800]= {":080000001A12040001000000C7:020000040002F8:0C2404000000E0004220AF00080037009C:102410003B1C090000000000FE3F09000000000016:10242000FE3F0900000000000000E900F8FF3A004C:10243000000006000FF82300F0FF230020A0B700E3:102440000000000081E0A8000600FC0021742000CC:1024500062042000730520008248780083487800D9:1024600042C7A90090F02300A4A6B70030002000C6:10247000C4A6B7000600FC002174200062042000FE:1024800073052000824878008348780042C7A8007E:102490000400280004118800044020001411880062:1024A0002062A80064052000441188002CA3EF00DE:1024B0000028EF00C442A800C462A800401F20000A:1024C0000212020001000000C462A900401F2000A7:1024D0000212020001000000C462A8005E120400A3:0824E000010000000040FE00B5:0200000401F009:10000000CF000000CF0000000700000081000000CA:10001000230000004000000017000000C3000000A3:00000001FF!"};
//rom char prog_text[880]= {":080000001A12040001000000C7:020000040002F8:0C2404000000E0004220AF00080037009C:102410003B1C090000000000FE3F09000000000016:10242000FE3F0900000000000000E900F8FF3A004C:10243000000006000FF82300F0FF230020A0B700E3:102440000000000081E0A8000600FC0021742000CC:1024500062042000730520008248780083487800D9:1024600042C7A90090F02300A4A6B70030002000C6:10247000C4A6B7000600FC002174200062042000FE:1024800073052000824878008348780042C7A8007E:102490000400280004118800044020001411880062:1024A0002062A80064052000441188002CA3EF00DE:1024B0000028EF00C442A800C462A800401F20000A:1024C0000212020001000000C462A900401F2000A7:1024D0000212020001000000C462A8005E120400A3:0824E000010000000040FE00B5:0200000401F009:10000000CF0000FFCF0000FF070000FF810000FFCE:10001000230000FF400000FF170000FFC30000FFA7:00000001FF!"};//with fuses blinky 1 fuses
//rom char prog_text[880]= {":080000001A12040001000000C7:020000040002F8:0C2404000000E0004220AF00080037009C:102410003B1C090000000000FE3F09000000000016:10242000FE3F0900000000000000E900F8FF3A004C:10243000000006000FF82300F0FF230020A0B700E3:102440000000000081E0A8000600FC0021742000CC:1024500062042000730520008248780083487800D9:1024600042C7A90090F02300A4A6B70030002000C6:10247000C4A6B7000600FC002174200062042000FE:1024800073052000824878008348780042C7A8007E:102490000400280004118800044020001411880062:1024A0002062A80064052000441188002CA3EF00DE:1024B0000028EF00C442A800C462A800401F20000A:1024C0000212020001000000C462A900401F2000A7:1024D0000212020001000000C462A8005E120400A3:0824E000010000000040FE00B5:0200000401F009:10000000CF0000FFCF0000FF030000FF810000FFD2:10001000230000FF400000FF170000FFC30000FFA7:00000001FF!"};//with fuses blinky 2 invalid fuses
rom char prog_text[880]= {":080000001A12040001000000C7:020000040002F8:0C2404000000E0004220AF00080037009C:102410003B1C090000000000FE3F09000000000016:10242000FE3F0900000000000000E900F8FF3A004C:10243000000006000FF82300F0FF230020A0B700E3:102440000000000081E0A8000600FC0021742000CC:1024500062042000730520008248780083487800D9:1024600042C7A90090F02300A4A6B70030002000C6:10247000C4A6B7000600FC002174200062042000FE:1024800073052000824878008348780042C7A8007E:102490000400280004118800044020001411880062:1024A0002062A80064052000441188002CA3EF00DE:1024B0000028EF00C442A800C462A800401F20000A:1024C0000212020001000000C462A900401F2000A7:1024D0000212020001000000C462A8005E120400A3:0824E000010000000040FE00B5:0200000401F009:10000000CF0000FFCF0000FF070000FF810000FFCE:10001000230000FF500000FF170000FFC30000FF97:00000001FF!"};//with fuses blinky 3 fuses WDTPRE is 50
//rom char prog_text[880]= {":080000001A12040001000000C7:020000040002F8:0C2404000000E0004220AF00080037009C:102410003B1C090000000000FE3F09000000000016:10242000FE3F0900000000000000E900F8FF3A004C:10243000000006000FF82300F0FF230020A0B700E3:102440000000000081E0A8000600FC0021742000CC:1024500062042000730520008248780083487800D9:1024600042C7A90090F02300A4A6B70030002000C6:10247000C4A6B7000600FC002174200062042000FE:1024800073052000824878008348780042C7A8007E:102490000400280004118800044020001411880062:1024A0002062A80064052000441188002CA3EF00DE:1024B0000028EF00C442A800C462A800401F20000A:1024C0000212020001000000C462A900401F2000A7:1024D0000212020001000000C462A8005E120400A3:0824E000010000000040FE00B5:00000001FF!"};//blinky no fuses
uint16 tmpcnt=0;
#endif

uint16 val;
enum BL_STATE{bl_state_waiting_for_cmd,bl_state_waiting_for_eol, bl_state_waiting_for_high_byte,bl_state_waiting_for_low_byte};
enum BL_BYTE_STATE{bl_byte_state_none,bl_byte_state_len,bl_byte_state_address_high,bl_byte_state_address_low,bl_byte_state_type,bl_byte_state_data,bl_byte_state_check};
BL_STATE bl_state=bl_state_waiting_for_cmd;
BL_BYTE_STATE bl_byte_state;
uint16 len;
uint16 address;
uint16 type;
uint8  data[256];
uint16 data_idx;
uint8 check;
uint16 page;
uint16 page_byte_offset;
uint16 i;
uint8 ram_org_reset_instruction[8];
bool org_reset_instruction_written;
uint32 address_full;
uint16 pagechange;
uint16 page_zero_received;

uint16 ch_from_uart;

#bankx
uint8 page_ram[2048];

#ifdef DEBUG
uint8 page_ram_tmp_debug[2048];
#endif

#banky
uint8 page_ram_tmp[2048];

#define myputc(ch) U1TXREG=ch;

void set_base_mem_location()
{
#asm
    //load pageram, and base memory location
    MOV page,W0;
    LSR W0,#6,W1;
    MOV W1,TTBLPAG;
    SL  W0,#10,W8;
    MOV page_ram,W5;
#endasm
}

void read_small_page()
{

#ifdef DEBUG
    printf("reading page %lu\n",page);
#endif

    set_base_mem_location();


#asm
    //do the copy to page ram

    //each page has 512 instructions. each instruction has 4 words = 2048 bytes
    //for fuses, page is 0x3E00 and [0x0032]==0xF8 then the page is just 8 instructions
    MOV #511,W2;
    MOV #0xF8,W3;
    CPSNE W1,W3;//if fuses page
    MOV #7,W2;
    DO W2,read_small_page_loop;
    TBLRDL [W8],[W5++];
read_small_page_loop:
    TBLRDH [W8++],[W5++];


#endasm



#ifdef DEBUG
#ifdef DEBUG_DISPLAY_READ_PROGRAM_MEMORY
    uint32 *instruction;
    int k=0;
    instruction=page_ram;
    for(i=0;i<512;i++)
    {
        if(!k)printf("\n%8X : ",i);
        printf("%8X ",*instruction);
        k++;k%=8;
        instruction++;
    }
    printf("\n");
#endif
#endif


}

void hex_digit_to_number()
{

#asm
    MOV ch_from_uart,W4;

    MOV     #0x30,W3
    CP      W3,W4
    BRA     GTU,elseletter
    MOV     #0x39,W3
    CP      W3,W4
    BRA     NC,elseletter
    SUB     #0x30,W4
    GOTO    ifdone
    elseletter:
    MOV     #0x41,W3
    CP      W3,W4
    BRA     GTU,elseclr
    MOV     #0x46,W3
    CP      W3,W4
    BRA     NC,elseclr
    SUB     #0x37,W4
    GOTO    ifdone
    elseclr:
    CLR     W4
    ifdone:

    MOV W4,ch_from_uart;



#endasm
}

void write_page_from_ram_to_flash()
{
#ifdef DEBUG
    printf("preparing to write to flash page %lu\n",page);
    if(page<BOOTLOADER_PAGE)printf("User program space\n");
    if(page==BOOTLOADER_PAGE)printf("Bootloader program space\n");
    if(page>BOOTLOADER_PAGE)printf("Fuses space\n");
#endif

    if((page_zero_received)&&(page==ORG_RESET_INSTRUCTION_PAGE))
    {
        //copy org reset instruction to page ram
        #asm
        MOV ram_org_reset_instruction,W3;
        MOV page_ram,W4;//!!! this could be removed as W4==page_ram from the if(page==ORG_RESET_INSTRUCTION_PAGE) statment. in pcwhd using ORG_RESET_INSTRUCTION_PAGE in asm is not nice either.??? not sure if that's true looking back 1month later ???
        MOV #2040,W0;
        ADD W4,W0,W4;
        REPEAT #3;
        MOV [W3++],[W4++];
        #endasm
        org_reset_instruction_written=true;
    }

    //erase and write the page except if its a fuse page then we just write one at a time

    set_base_mem_location();


    if(page<BOOTLOADER_PAGE)
    {


        //start erase page

#ifdef DEBUG
        printf("eraseing page %lu\n",page);
        set_base_mem_location();
#endif

        //setup page erase (512 instructions, 2048 bytes but the first byte is a dummy byte)
        NVMCON=0x4042;

        //set_base_mem_location();

#asm
        TBLWTL W8,[W8];

#endasm


        //do erase
#asm
        DISI #5;
#endasm
        NVMKEY=0x55;
        NVMKEY=0xAA;
        NVMCONbits.WR=1;
#asm
        NOP;
        NOP;
#endasm


#ifdef DEBUG
        NVMCONbits.WREN=0;//disable erase operations
        printf("verifing erase\n");
        for(i=0;i<2048;i++)page_ram_tmp_debug[i]=page_ram[i];
        read_small_page();
        uint32 *instruction=(uint32*)page_ram;
        for(i=0;i<512;i++)
        {
            if((*instruction)!=0x00FFFFFF)
            {
                printf("erasing failed\n");
                printf("instruction number in page=%lu instruction read=%2X\n",i,(*instruction));
                break;
            }
        }
        if(i==512)
        {
            printf("erasure ok\n");
        }
        for(i=0;i<2048;i++)page_ram[i]=page_ram_tmp_debug[i];

        set_base_mem_location();

#endif

        //end erase


#ifdef DEBUG
        printf("writing page %lu\n",page);
#endif


        //start write

        NVMCON=0x4001;//setup for write

#asm

        //write page from ram to latch and write it to the flash
        DO #7,write_small_page_loop_out;
        DO #63,write_small_page_loop;
        MOV [W5++],W4;
        MOV [W5++],W3;
        TBLWTL W4,[W8];// Write PM low word into program latch
write_small_page_loop:
        TBLWTH W3,[W8++];// Write PM high byte into program latch

        //write the latched data to the flash
        DISI #5;
#endasm
        NVMKEY=0x55;
        NVMKEY=0xAA;
        NVMCONbits.WR=1;
#asm
        NOP;
write_small_page_loop_out:
        NOP;

#endasm
        NVMCONbits.WREN=0;//disable write operations

#ifdef DEBUG
        printf("verifing write\n");
        for(i=0;i<2048;i++)page_ram_tmp_debug[i]=page_ram[i];//save
        read_small_page();
        uint32 *page_ram_instruction=page_ram;
        uint32 *page_ram_instruction_tmp_debug=page_ram_tmp_debug;
        for(i=0;i<512;i++)
        {
            if(page_ram_instruction[i]!=(page_ram_instruction_tmp_debug[i]&0x00FFFFFF))//ignore the dummy variable during the readback
            {
                printf("written and read differ\n");
                printf("pos=%lu written=%8X read=%8X\n",i,page_ram_instruction_tmp_debug[i],page_ram_instruction[i]);
                break;
            }
        }
        if(i==512)
        {
            printf("written ok\n");
        }
        for(i=0;i<2048;i++)page_ram[i]=page_ram_tmp_debug[i];//restore
#endif



    }
     else
     {


#ifdef DEBUG
        printf("writing fuses %lu\n",page);
        set_base_mem_location();
#endif

        NVMCON=0x4000;//setup for write. fuses seem to only be able to be written one at a time

#asm

        //write fuse one at a time to the flash if fuse is different
        DO #7,write_small_page_loopaa;
        TBLRDL [W8++],W4;
        CP W4,[W5++];
        BRA Z,write_small_page_loopaa;
        TBLWTL [--W5],[--W8];// Write PM low word into program latch
        TBLWTH [++W5],[W8++];

        //write the latched data to the flash
        DISI #5;
#endasm
        NVMKEY=0x55;
        NVMKEY=0xAA;
        NVMCONbits.WR=1;
#asm
        NOP;
        NOP;
write_small_page_loopaa:
        ADD W5,#2,W5;

#endasm
        NVMCONbits.WREN=0;//disable write operations

#ifdef DEBUG
        printf("verifing fuses write\n");
        for(i=0;i<2048;i++)page_ram_tmp_debug[i]=page_ram[i];//save
        read_small_page();
        uint32 *page_ram_instruction=page_ram;
        uint32 *page_ram_instruction_tmp_debug=page_ram_tmp_debug;
        for(i=0;i<512;i++)
        {
            if(page_ram_instruction[i]!=(page_ram_instruction_tmp_debug[i]&0x00FFFFFF))//ignore the dummy variable during the readback
            {
                printf("written and read differ\n");
                printf("pos=%lu written=%8X read=%8X\n",i,page_ram_instruction_tmp_debug[i],page_ram_instruction[i]);
                break;
            }
        }
        if(i==512)
        {
            printf("written ok\n");
        }
        for(i=0;i<2048;i++)page_ram[i]=page_ram_tmp_debug[i];//restore
#endif


     }


    //begin verification of write and tell user if its a negitive result (uses 15 instructions)

    //copy page ram to tmp page ram
    #asm
    MOV page_ram,W3;
    MOV page_ram_tmp,W4;

    //copies but sets the dummy byte to 0
    CLR W0;
    DO #1023,ftfy;
    MOV [W3++],[W4++];
    MOV.B [W3++],[W4++];
    MOV.B W0,[W4++];
ftfy:
    INC W3,W3;

    #endasm


    //read
    read_small_page();

    //compare ram pages
    #asm
    MOV page_ram,W8;
    MOV page_ram_tmp,W10;
    BCLR CPU_SR,1;
    DO #1023,verify_small_page_loop;
    CLR A,[W8]+=2,W6,[W10]+=2,W7;
    CPSEQ W6,W7;
    BSET CPU_SR,1;
verify_small_page_loop:
    // NOP;//can miss as the next instruction is just a move instruction and we arn't using that reg
#endasm

    //tx nack if write failed
    #asm
    MOV HASH_WRITE_FAIL,W0;//verification failed char
    BTSC CPU_SR,1;
    MOV W0,U1TXREG;
    #endasm

}

void main()
{

    //init variables that are really important
    #asm
    CLR address_full+2;
    #endasm
    org_reset_instruction_written=false;
    page_zero_received=false;
    page=0xFFFF;

    //init leds
    #ifdef USE_LEDS
    RED_LED_DIRECTION=0;
    GREEN_LED_DIRECTION=0;
    #endif
    
    //green on
    GREEN_LED_ON();

    // Configure Oscillator
    PLLFBD=SETTINGS_PLLFBD;               // M
    CLKDIV=((SETTINGS_PLLPOST<<6)|SETTINGS_PLLPRE);// N1,N2
    OSCTUN=SETTINGS_OSCTUN;               // Adjust to correct frequency
    
    //switch to external osc with PLL
    #asm
    MOV #0x3,W0
    MOV #0x743,W1
    MOV #0x78,W2
    MOV #0x9A,W3
    MOV.B W2,[W1]
    MOV.B W3,[W1]
    MOV.B W0,OSCCON+1
    MOV #0x742,W1
    MOV #0x46,W2
    MOV #0x57,W3
    MOV.B W2,[W1]
    MOV.B W3,[W1]
    BSET OSCCON,#0
    waittilldone:
    BTSC OSCCON,#0
    BRA waittilldone;
    #endasm

    //setup 32 timer used for timeouts
    T3CON = 0; // Stop any 16-bit Timer3 operation
    T2CON = 0; // Stop any 16/32-bit Timer2 operation
    T3IE = 0; // Disable Timer3 interrupt
    T2CON = 0x0018;
    TMR3 = 0x00; // Clear 32-bit Timer (msw)
    TMR2 = 0x00; // Clear 32-bit Timer (lsw)
    PR3 = TIMEOUT_MSW;// Load 32-bit period value (msw)
    PR2 = TIMEOUT_LSW;// Load 32-bit period value (lsw)
    T3IF = 0; // Clear Timer3 Interrupt Flag
    T2CONbits.TON = 1; // Start 32-bit Timer
    
    GREEN_LED_OFF();

    //blink red led and wait a bit for the ESC char
    uint16 timeout=0xFFFF;
    #asm
    MOV #3,W12;//number-1 of SYNCS to print before timeing out. this is about 1 second
    DO W12,esc_wait_loop;
    #endasm
    while((!T3IF)&&(!U1RXDA));
    #asm
    BTSS  U1RXDA
    BRA   skip_if_nochar
    MOV   U1RXREG,W0
    MOV   HASH_ESC,W4;//is the esc char pressed
    CP.B  W4,W0
    BRA   NZ,skip_if_nochar//if so...
    BSET CORCON,11;//exit loop early
    CLR   timeout;//notify whoever that we are exiting due to the esc char push
    skip_if_nochar:
    #endasm
    U1TXREG=SYNC;
    T3IF=0;
    #asm
    esc_wait_loop:
    BTG RED_LED;
    #endasm

#ifdef SIMULATOR
    //simulate that esc was pushed
    timeout=0;
#endif

    RED_LED_OFF();

    //if timeout then call org prog
    if(timeout)
    {
#ifdef DEBUG
        printf("time out->users instruction\n\r");
#endif
        //run the users code
        #asm;
        GOTO ORG_GOTO_INSTRUCTION_ADDRESS;
        #endasm;
    }



    //to tell big computer who we are
    U1TXREG=SOH;
    U1TXREG=BL_ID_LOW;//cpu and bootloader id
    U1TXREG=BL_ID_HIGH;//cpu and bootloader id
    while(!U1TRMT);//we have 4 char fifo buffer
    U1TXREG=BL_ID_CHK;//some sort of crc (it's the last 8 bits of Iso3309)

    //to tell big computer to start talking
    U1TXREG=READY;

    while(true)
    {


#ifdef SIMULATOR
        //simulate sending data to bootloader
        while(!U1TRMT);//so that the rx of ch is not too fast for sim
        ch_from_uart=0;
        ch_from_uart=prog_text[tmpcnt];tmpcnt++;
        if(ch_from_uart=='!')
        {
#ifdef DEBUG
            printf("end---\r\n");
#endif
            while(true)
            {

            }
        }
#else


#ifdef DEBUG
        ch_from_uart=0;
        ch_from_uart=getch();
#else
        //wait for char and goto user start if we time out
        TMR2=0;
        TMR3=0;
        while((!T3IF)&&(!U1RXDA));
        if(T3IF)
        {
            GREEN_LED_OFF();
            #asm
            GOTO ORG_GOTO_INSTRUCTION_ADDRESS;
            #endasm
        }
        #asm
        MOV     U1RXREG,W0;
        MOV   W0,ch_from_uart;
        #endasm
#endif
#endif

        switch (bl_state)
        {
        case bl_state_waiting_for_cmd:
            if(ch_from_uart==':')
            {
                GREEN_LED_ON();
                bl_state=bl_state_waiting_for_high_byte;
                bl_byte_state=bl_byte_state_len;
            }
            break;
        case bl_state_waiting_for_high_byte:
            bl_state=bl_state_waiting_for_low_byte;
            hex_digit_to_number();
            val=ch_from_uart<<4;
            break;
        case bl_state_waiting_for_low_byte:
            bl_state=bl_state_waiting_for_high_byte;
            hex_digit_to_number();
            val|=ch_from_uart;
            switch (bl_byte_state)
            {
            case bl_byte_state_len:
                bl_byte_state=bl_byte_state_address_high;
                len=val;
                check=val;
                break;
            case bl_byte_state_address_high:
                bl_byte_state=bl_byte_state_address_low;
                address=val<<8;
                check+=val;
                break;
            case bl_byte_state_address_low:
                bl_byte_state=bl_byte_state_type;
                address|=val;
                check+=val;
                break;
            case bl_byte_state_type:
                bl_byte_state=bl_byte_state_data;
                type=val;
                check+=val;
                data_idx=0;
                break;
            case bl_byte_state_data:

                if(len)
                {
                    len--;
                    data[data_idx]=val;
                    data_idx++;
                    check+=val;
                }
                 else
                 {
                     GREEN_LED_OFF();
                     bl_state=bl_state_waiting_for_cmd;
                     #asm
                     MOV.B check,W0;
                     NEG.B W0,W0;
                     MOV.B W0,check;
                     #endasm
                     if(check!=val)
                     {
#ifdef DEBUG
                        for(i=0;i<data_idx;i++)
                        {
                            printf("%2X",data[i]);
                        }
                        printf(" bad checksum check=%u val=%lu\r\n",check,val);
#endif
                        myputc(NAKLOD);
                        break;
                     }

                     myputc(ACKLOD);

                     //line is valid let's work on it
                     switch (type)
                     {
                     case 1://EOF
#ifdef DEBUG
                         printf("EOF\r\n");
#endif

                         //write the last page and instruction if needed
                         if(page!=0xFFFF)
                         {
                             write_page_from_ram_to_flash();

                             //if we haven't written the goto instruction
                             if((page_zero_received)&&(!org_reset_instruction_written))
                             {
#ifdef DEBUG
                                 printf("page zero received but haven't written goto instruction yet\r\n");
#endif
                                 page=ORG_RESET_INSTRUCTION_PAGE;
                                 read_small_page();
                                 write_page_from_ram_to_flash();
                             }
                         }

                         myputc(EOT);
                         while(!U1TRMT);

#ifdef DEBUG
                         printf("finished burning->users instruction\n\r");
#endif
                         //run the users code
                         #asm;
                         GOTO ORG_GOTO_INSTRUCTION_ADDRESS;
                         #endasm;

                         break;
                     case 4://to tell us to update the high 16bit address field
                         #asm
                         MOV data,W1;
                         MOV [W1],W0;
                         SWAP W0;
                         MOV W0,address_full+2;
                         #endasm
#ifdef DEBUG
                         printf("\r\nloaded new high address %4X\n\r",(uint16)(address_full>>16));
#endif
                         break;
                     case 0://to tell us what to write to the flash
                         //update the low 16bits of the address
                         #asm
                         MOV address,W0;
                         MOV W0,address_full;
                         #endasm

#ifdef DEBUG
                         //debug print the address and instructions
                         uint32 *instructions=(uint32*)data;
                         uint32 address_full_tmp=address_full>>1;
                         printf("\r\n");
                         for(i=0;i<data_idx/4;i++)
                         {
                             printf("%8X    %8X\r\n",address_full_tmp,instructions[i]);
                             address_full_tmp+=2;
                         }
#endif

                         //proposed_page=address_full>>11;
                         uint16 proposed_page;
                         #asm
                         MOV address_full+2,W2;
                         MOV address_full,W1;
                         LSR W1,#11,W1;
                         SL  W2,#5,W2;
                         IOR W1,W2,W0;
                         MOV W0,proposed_page;
                         #endasm

#ifdef DEBUG
                         printf("page %lu proposed page %lu\r\n",page,proposed_page);
#endif
                         //check that we have something and dont write over us
                         if((!data_idx)||(proposed_page==BOOTLOADER_PAGE))
                         {
                             myputc(ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA);
                             break;
                         }

                         //if we have a change of page then write the old and read the new page
                         pagechange=0;
                         if(page!=proposed_page)//(address_full>>11))
                         {
                             pagechange=0xFFFF;

                             //write old page
                             if(page!=0xFFFF)write_page_from_ram_to_flash();

                             //calc new page in words not bytes
                             page=proposed_page;//(address_full>>11);

                             //read new page
                             read_small_page();
                         }

                         //calc page byte offset
                         page_byte_offset=(address_full)&0x7FF;

                         //check we dont overwrite the goto instruction
                         if((proposed_page==ORG_RESET_INSTRUCTION_PAGE)&&((page_byte_offset+data_idx)>2040))
                         {
                             myputc(ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA);
                             break;
                         }

#ifdef DEBUG
                         printf("address_full=%8X\r\n",address_full);
#endif

                         //write instructions to paged ram
                         if(address_full==0)
                         {

                             //check that we have the goto instruction
                             if(data_idx<8)
                             {
                                 myputc(ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA);
                                 break;
                             }

                             #asm

                             SETM page_zero_received;

                             MOV data,W4;
                             MOV ram_org_reset_instruction,W3;

                             REPEAT #7;
                             MOV.B [W4++],[W3++];

                             MOV page_ram,W3;
                             MOV page_byte_offset,W0;
                             ADD W3,W0,W3;
                             ADD W3,#8,W3;

                             MOV data_idx,W12;
                             SUB W12,#8,W12;
                             CP0 W12
                             BRA Z,skip_if_nothing;

                             SUB W12,#1,W12;
                             REPEAT W12;
                             MOV.B [W4++],[W3++];

                             skip_if_nothing:
                             #endasm


                         }
                          else
                          {

                             #asm
                             MOV data_idx,W12;
                             SUB W12,#1,W12;
                             MOV page_ram,W3;
                             MOV page_byte_offset,W0;
                             ADD W3,W0,W3;
                             MOV data,W4
                             REPEAT W12;
                             MOV.B [W4++],[W3++];
                             #endasm

                          }

                         //if(pagechange)myputc(READY); in asm but 1 less instruction
                         #asm
                         MOV HASH_READY,W0;
                         BTSC pagechange,#0;
                         MOV W0,U1TXREG;
                         #endasm

                         break;
                     default:
#ifdef DEBUG
                         printf("unknown type=%ld\r\n",type);
#endif
                         myputc(ERROR_UNKNOWN_TYPE);
                         break;
                     }

                 }

                break;
            default:
                break;
            }

            break;
        default:
            break;
        }


    }

}






