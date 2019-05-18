#ifndef _PICREGS_
#define _PICREGS_



//FLASH memory

#word NVMCON = getenv("sfr:NVMCON")
#word NVMKEY = getenv("sfr:NVMKEY")
#word TTBLPAG = 0x0032 //#byte/word TBLPAG = 0x0032 //this does not work, bug with pcwhd!. TBLPAG is defined somewhere but doesn't seem to be a byte or a wird. so have to use a different name
struct
{
    int NVMOP:4;
    int Reserved_2:2;
    int ERASE:1;
    int Reserved:6;
    int WRERR:1;
    int WREN:1;
    int WR:1;
}NVMCONbits;
#locate NVMCONbits = getenv("SFR:NVMCON")

//Digital ports

#word PORTA = getenv("sfr:PORTA")
#word PORTB = getenv("sfr:PORTB")

#word TRISA = getenv("sfr:TRISA")
#word TRISB = getenv("sfr:TRISB")

#word LATA = getenv("sfr:LATA")
#word LATB = getenv("sfr:LATB")

//clock
#word OSCCON = getenv("sfr:OSCCON")
#word PLLFBD = getenv("sfr:PLLFBD")
#word CLKDIV = getenv("sfr:CLKDIV")
#byte OSCCONH= OSCCON
#byte OSCCONL= OSCCON+1
struct
{
    int PLLPRE:5;
    int Reserved:1;
    int PLLPOST:2;
    int FRCDIV:3;
    int DOZEN:1;
    int DOZE:3;
    int ROI:1;
}CLKDIVbits;
#locate CLKDIVbits = getenv("SFR:CLKDIV")
#word OSCTUN = getenv("sfr:OSCTUN")

//bit reversal
#word XBREV = getenv("sfr:XBREV")

//modular addressing
#word XMODSRT = getenv("sfr:XMODSRT")
#word XMODEND = getenv("sfr:XMODEND")
#word YMODSRT = getenv("sfr:YMODSRT")
#word YMODEND = getenv("sfr:YMODEND")
#word MODCON = getenv("sfr:MODCON")
// Define bit structures for MODCON Register
struct
{
   int XWM:4;
   int YWM:4;
   int BWM:4;
   int Reserved:2;
   int YMODEN:1;
   int XMODEN:1;
}MODCONbits;
#locate MODCONbits = getenv("SFR:MODCON")

//MAC
#word CORCON = getenv("SFR:CORCON")
#word ACCAU = getenv("SFR:ACCAU")
#word ACCAH = getenv("SFR:ACCAH")
#word ACCAL = getenv("SFR:ACCAL")
#word ACCBU = getenv("SFR:ACCBU")
#word ACCBH = getenv("SFR:ACCBH")
#word ACCBL = getenv("SFR:ACCBL")

//CPU flags
#word CPU_SR = getenv("SFR:SR")

//IF
#word IEC4=getenv("SFR:IEC4")
#word IFS4=getenv("SFR:IFS4")

//DAC
#word DAC1LDAT = getenv("SFR:DAC1LDAT")
#word DAC1RDAT = getenv("SFR:DAC1RDAT")
#word ACLKCON = getenv("SFR:ACLKCON")
#word DAC1STAT = getenv("SFR:DAC1STAT")
#word DAC1DFLT = getenv("SFR:DAC1DFLT")
#word DAC1CON = getenv("SFR:DAC1CON")
#bit DAC1CON_EN = DAC1CON.15
struct
{
    int REMPTY:1;
    int RFULL:1;
    int RITYPE:1;
    int Unimplimented:2;
    int RMVOEN:1;
    int Unimplimented_2:1;
    int ROEN:1;
    int LEMPTY:1;
    int LFULL:1;
    int LITYPE:1;
    int Unimplimented_3:2;
    int LMVOEN:1;
    int Unimplimented_4:1;
    int LOEN:1;
}DAC1STATbits;
struct
{
    int Unimplimented:7;
    int ASRCSEL:1;
    int APSTSCLR:3;
    int AOSCMD:2;
    int SELACLK:1;
    int Unimplimented_2:2;
}ACLKCONbits;
struct
{
    int DACFDIV:7;
    int Unimplimented:1;
    int FORM:1;
    int Unimplimented_2:3;
    int AMPON:1;
    int DACSIDL:1;
    int Unimplimented_3:1;
    int DACEN:1;
}DAC1CONbits;

//UART1
#byte U1TXREG = getenv("sfr:U1TXREG")
#word U1RXREG = getenv("sfr:U1RXREG")
#word U1STA = getenv("sfr:U1STA")
#bit  U1TXBF = U1STA.9
#bit  U1RXDA = U1STA.0
#bit  U1TRMT = U1STA.8


//MISC
#word DCOUNT = getenv("SFR:DCOUNT")

//INT
#word MCU_IFS4 = getenv("SFR:IFS4")
#word MCU_IEC4 = getenv("SFR:IEC4")

//CRC
#word CRCCON = getenv("SFR:CRCCON")
#word CRCXOR = getenv("SFR:CRCXOR")
#word CRCDAT = getenv("SFR:CRCDAT")
#word CRCWDAT = getenv("SFR:CRCWDAT")
struct
{
    int PLEN:4;//bit 0
    int CRCGO:1;
    int Unimplimented:1;
    int CRCMPT:1;
    int CRCFUL:1;
    int VWORD:5;
    int CSIDL:1;
    int Unimplimented_2:2;//bit 15
}CRCCONbits;
#locate CRCCONbits = getenv("SFR:CRCCON")
#bit  CRCIF = MCU_IFS4.3
#bit  CRCIE = IEC4.3

//ADC
#word AD1CON1 = getenv("SFR:AD1CON1")
#word AD1CON2 = getenv("SFR:AD1CON2")
#word AD1CON3 = getenv("SFR:AD1CON3")
#word AD1CON4 = getenv("SFR:AD1CON4")
#word AD1CHS123 = getenv("SFR:AD1CHS123")
#word AD1CHS0 = getenv("SFR:AD1CHS0")
#word AD1PCFGL = getenv("SFR:AD1PCFGL")
#word AD1CSSL = getenv("SFR:AD1CSSL")
#bit AD1CON1_ADON = AD1CON1.15

#word DMA1CON = getenv("SFR:DMA1CON")
#word DMA1REQ = getenv("SFR:DMA1REQ")
#word DMA1STA = getenv("SFR:DMA1STA")
#word DMA1STB = getenv("SFR:DMA1STB")
#word DMA1PAD = getenv("SFR:DMA1PAD")
#word DMA1CNT = getenv("SFR:DMA1CNT")
#word DMACS0 = getenv("SFR:DMACS0")
#word DMACS1 = getenv("SFR:DMACS1")
#bit  DMACS1_PPST0 = DMACS1.0
#bit  DMACS1_PPST1 = DMACS1.1
#bit  DMACS1_PPST2 = DMACS1.2
#bit  DMACS1_PPST3 = DMACS1.3
#bit  DMACS1_PPST4 = DMACS1.4
#bit  DMACS1_PPST5 = DMACS1.5
#bit  DMACS1_PPST6 = DMACS1.6
#bit  DMACS1_PPST7 = DMACS1.7

#word IFS0 = getenv("SFR:IFS0")
#word IEC0 = getenv("SFR:IEC0")

#word T1CON = getenv("SFR:T1CON")
#word TMR1 = getenv("SFR:TMR1")
#word PR1 = getenv("SFR:PR1")
#bit  T1IF = IFS0.3
#bit  T1IE = IEC0.3

#word T2CON = getenv("SFR:T2CON")
#word TMR2 = getenv("SFR:TMR2")
#word PR2 = getenv("SFR:PR2")
#bit  T2IF = IFS0.7
#bit  T2IE = IEC0.7

#word T3CON = getenv("SFR:T3CON")
#word TMR3 = getenv("SFR:TMR3")
#word PR3 = getenv("SFR:PR3")
#bit  T3IF = IFS0.8
#bit  T3IE = IEC0.8

struct
{
    int T3IP:3;//bit 0
    int Unimplimented:1;
    int SPI1EIP:3;
    int Unimplimented_2:1;
    int SPI1IP:3;
    int Unimplimented_3:1;
    int U1RXIP:3;
    int Unimplimented_4:1;//bit 15
}IPC2bits;
#locate IPC2bits = getenv("SFR:IPC2")

struct
{
    int Unimplimented:1;//bit 0
    int TCS:1;
    int Unimplimented_2:1;
    int T32:1;
    int TCKPS:2;
    int TGATE:1;
    int Unimplimented_3:6;
    int TSIDL:1;
    int Unimplimented_4:1;
    int TON:1;//bit 15
}T2CONbits;
#locate T2CONbits = getenv("SFR:T2CON")

struct
{
    int CORCONIF:1;//bit 0
    int RND:1;
    int PSV:1;
    int IPL3:1;
    int ACCSAT:1;
    int SATDW:1;
    int SATB:1;
    int SATA:1;
    int DL:3;
    int EDT:1;
    int US:1;
    int Reserved:3;//bit 15
}CORCONbits;
#locate CORCONbits = getenv("SFR:CORCON")

#endif


