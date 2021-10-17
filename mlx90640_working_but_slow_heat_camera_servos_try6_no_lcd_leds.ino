#include <Adafruit_MLX90640.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

float frame[32*24]; // buffer for full frame of temperatures

Adafruit_MLX90640 mlx;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_LEFT  400//460
#define SERVO_CENTER  280//460
#define SERVO_RIGHT  120//100
#define SERVO_TURN_LEFT  +5
#define SERVO_TURN_RIGHT  -5

#define SERVONUM 0

#define LEFT 1
#define RIGHT 2
#define CENTER 3

#define LEFT_EYE 1
#define LEFT_EYE_RED 1
#define LEFT_EYE_GREEN 2
#define LEFT_EYE_BLUE 3

#define RIGHT_EYE 2
#define RIGHT_EYE_RED 4
#define RIGHT_EYE_GREEN 5
#define RIGHT_EYE_BLUE 6

int motor_adjustment = SERVO_CENTER;

//Servos
int calc_new_position(int adjustment) {

  int normalized_adj=adjustment;
  
  if(adjustment > SERVO_LEFT ) {
    normalized_adj=SERVO_LEFT;
  }
  if(adjustment < SERVO_RIGHT ) {
    normalized_adj=SERVO_RIGHT;
  }
  return normalized_adj;
}
/*
int set_led( int led, int direction, int r, int g, int b ){

    int r_m=0;
    int g_m=0;
    int b_m=0;
    
  if ( direction == RIGHT ){
    Serial.print( "Turn Right: ");
     Serial.println( direction);
    int r_m = map(r, SERVO_RIGHT, SERVO_LEFT, 10, 255);
    int g_m = map(g, SERVO_RIGHT, SERVO_LEFT, 10, 255);
    int b_m = map(b, SERVO_RIGHT, SERVO_LEFT, 10, 255);
  }
  else if ( direction == LEFT ){
        Serial.print( "Turn Left");
Serial.println( direction);
    int r_m = map(r, SERVO_LEFT,SERVO_RIGHT, 10, 255);
    int g_m = map(g, SERVO_LEFT,SERVO_RIGHT, 10, 255);
    int b_m = map(b, SERVO_LEFT,SERVO_RIGHT, 10, 255);
  }
  else {
    int r_m=255;
    int g_m=255;
    int b_m=255;
  }
  Serial.println( r_m);
*/
int set_led( int led, int r, int g, int b ){
  if( led == LEFT_EYE ){
    pwm.setPWM(LEFT_EYE_RED  , 0, r);
    pwm.setPWM(LEFT_EYE_GREEN, 0, g);
    pwm.setPWM(LEFT_EYE_BLUE , 0, b);
  }
  else if ( led == RIGHT_EYE ) {
    pwm.setPWM(RIGHT_EYE_RED  , 0, r);
    pwm.setPWM(RIGHT_EYE_GREEN, 0, g);
    pwm.setPWM(RIGHT_EYE_BLUE , 0, b);    
  }
}

void setup() {

  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);

  //Wait for camera
  if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found!");
    while (1) delay(10);
  }
  Serial.println("MLX90640 found!");

  //Adjust refresh rate ?3.1.3. int MLX90640_SetRefreshRate (uint8_t slaveAddr, uint8_t refreshRate) 
  //mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_16_HZ);
  Wire.setClock(1000000); // max 1 MHz

      
  //mlx.setMode(MLX90640_INTERLEAVED);
  mlx.setMode(MLX90640_CHESS);

  //Servos
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates
  pwm.setPWM(SERVONUM, 0, SERVO_LEFT);
  set_led(LEFT_EYE,255,0,0);
  set_led(RIGHT_EYE,255,0,0);
  delay(5000);
  
  pwm.setPWM(SERVONUM, 0, SERVO_CENTER);
  set_led(LEFT_EYE,0,255,0);
  set_led(RIGHT_EYE,0,255,0);
  delay(5000);
  
  pwm.setPWM(SERVONUM, 0, SERVO_RIGHT);
  set_led(LEFT_EYE, 0,0,255);
  set_led(RIGHT_EYE,0,0,255);

  delay(5000);
    
}



void loop() {
  if (mlx.getFrame(frame) != 0) {
    Serial.println("Failed");
    return;
  }

  float left_heat=0;
  float center_heat=0;
  float right_heat=0;

  left_heat   = frame[22] + frame[23] + frame[24] + frame[25] + frame[26] + frame[27] + frame[28] + frame[29] + frame[30] + frame[31] + frame[54] + frame[55] + frame[56] + frame[57] + frame[58] + frame[59] + frame[60] + frame[61] + frame[62] + frame[63] + frame[86] + frame[87] + frame[88] + frame[89] + frame[90] + frame[91] + frame[92] + frame[93] + frame[94] + frame[95] + frame[118] + frame[119] + frame[120] + frame[121] + frame[122] + frame[123] + frame[124] + frame[125] + frame[126] + frame[127] + frame[150] + frame[151] + frame[152] + frame[153] + frame[154] + frame[155] + frame[156] + frame[157] + frame[158] + frame[159] + frame[182] + frame[183] + frame[184] + frame[185] + frame[186] + frame[187] + frame[188] + frame[189] + frame[190] + frame[191] + frame[214] + frame[215] + frame[216] + frame[217] + frame[218] + frame[219] + frame[220] + frame[221] + frame[222] + frame[223] + frame[246] + frame[247] + frame[248] + frame[249] + frame[250] + frame[251] + frame[252] + frame[253] + frame[254] + frame[255] + frame[278] + frame[279] + frame[280] + frame[281] + frame[282] + frame[283] + frame[284] + frame[285] + frame[286] + frame[287] + frame[310] + frame[311] + frame[312] + frame[313] + frame[314] + frame[315] + frame[316] + frame[317] + frame[318] + frame[319] + frame[342] + frame[343] + frame[344] + frame[345] + frame[346] + frame[347] + frame[348] + frame[349] + frame[350] + frame[351] + frame[374] + frame[375] + frame[376] + frame[377] + frame[378] + frame[379] + frame[380] + frame[381] + frame[382] + frame[383] + frame[406] + frame[407] + frame[408] + frame[409] + frame[410] + frame[411] + frame[412] + frame[413] + frame[414] + frame[415] + frame[438] + frame[439] + frame[440] + frame[441] + frame[442] + frame[443] + frame[444] + frame[445] + frame[446] + frame[447] + frame[470] + frame[471] + frame[472] + frame[473] + frame[474] + frame[475] + frame[476] + frame[477] + frame[478] + frame[479] + frame[502] + frame[503] + frame[504] + frame[505] + frame[506] + frame[507] + frame[508] + frame[509] + frame[510] + frame[511] + frame[534] + frame[535] + frame[536] + frame[537] + frame[538] + frame[539] + frame[540] + frame[541] + frame[542] + frame[543] + frame[566] + frame[567] + frame[568] + frame[569] + frame[570] + frame[571] + frame[572] + frame[573] + frame[574] + frame[575] + frame[598] + frame[599] + frame[600] + frame[601] + frame[602] + frame[603] + frame[604] + frame[605] + frame[606] + frame[607] + frame[630] + frame[631] + frame[632] + frame[633] + frame[634] + frame[635] + frame[636] + frame[637] + frame[638] + frame[639] + frame[662] + frame[663] + frame[664] + frame[665] + frame[666] + frame[667] + frame[668] + frame[669] + frame[670] + frame[671] + frame[694] + frame[695] + frame[696] + frame[697] + frame[698] + frame[699] + frame[700] + frame[701] + frame[702] + frame[703] + frame[726] + frame[727] + frame[728] + frame[729] + frame[730] + frame[731] + frame[732] + frame[733] + frame[734] + frame[735] + frame[758] + frame[759] + frame[760] + frame[761] + frame[762] + frame[763] + frame[764] + frame[765] + frame[766] + frame[767];
  center_heat = frame[11] + frame[12] + frame[13] + frame[14] + frame[15] + frame[16] + frame[17] + frame[18] + frame[19] + frame[20] + frame[21] + frame[43] + frame[44] + frame[45] + frame[46] + frame[47] + frame[48] + frame[49] + frame[50] + frame[51] + frame[52] + frame[53] + frame[75] + frame[76] + frame[77] + frame[78] + frame[79] + frame[80] + frame[81] + frame[82] + frame[83] + frame[84] + frame[85] + frame[107] + frame[108] + frame[109] + frame[110] + frame[111] + frame[112] + frame[113] + frame[114] + frame[115] + frame[116] + frame[117] + frame[139] + frame[140] + frame[141] + frame[142] + frame[143] + frame[144] + frame[145] + frame[146] + frame[147] + frame[148] + frame[149] + frame[171] + frame[172] + frame[173] + frame[174] + frame[175] + frame[176] + frame[177] + frame[178] + frame[179] + frame[180] + frame[181] + frame[203] + frame[204] + frame[205] + frame[206] + frame[207] + frame[208] + frame[209] + frame[210] + frame[211] + frame[212] + frame[213] + frame[235] + frame[236] + frame[237] + frame[238] + frame[239] + frame[240] + frame[241] + frame[242] + frame[243] + frame[244] + frame[245] + frame[267] + frame[268] + frame[269] + frame[270] + frame[271] + frame[272] + frame[273] + frame[274] + frame[275] + frame[276] + frame[277] + frame[299] + frame[300] + frame[301] + frame[302] + frame[303] + frame[304] + frame[305] + frame[306] + frame[307] + frame[308] + frame[309] + frame[331] + frame[332] + frame[333] + frame[334] + frame[335] + frame[336] + frame[337] + frame[338] + frame[339] + frame[340] + frame[341] + frame[363] + frame[364] + frame[365] + frame[366] + frame[367] + frame[368] + frame[369] + frame[370] + frame[371] + frame[372] + frame[373] + frame[395] + frame[396] + frame[397] + frame[398] + frame[399] + frame[400] + frame[401] + frame[402] + frame[403] + frame[404] + frame[405] + frame[427] + frame[428] + frame[429] + frame[430] + frame[431] + frame[432] + frame[433] + frame[434] + frame[435] + frame[436] + frame[437] + frame[459] + frame[460] + frame[461] + frame[462] + frame[463] + frame[464] + frame[465] + frame[466] + frame[467] + frame[468] + frame[469] + frame[491] + frame[492] + frame[493] + frame[494] + frame[495] + frame[496] + frame[497] + frame[498] + frame[499] + frame[500] + frame[501] + frame[523] + frame[524] + frame[525] + frame[526] + frame[527] + frame[528] + frame[529] + frame[530] + frame[531] + frame[532] + frame[533] + frame[555] + frame[556] + frame[557] + frame[558] + frame[559] + frame[560] + frame[561] + frame[562] + frame[563] + frame[564] + frame[565] + frame[587] + frame[588] + frame[589] + frame[590] + frame[591] + frame[592] + frame[593] + frame[594] + frame[595] + frame[596] + frame[597] + frame[619] + frame[620] + frame[621] + frame[622] + frame[623] + frame[624] + frame[625] + frame[626] + frame[627] + frame[628] + frame[629] + frame[651] + frame[652] + frame[653] + frame[654] + frame[655] + frame[656] + frame[657] + frame[658] + frame[659] + frame[660] + frame[661] + frame[683] + frame[684] + frame[685] + frame[686] + frame[687] + frame[688] + frame[689] + frame[690] + frame[691] + frame[692] + frame[693] + frame[715] + frame[716] + frame[717] + frame[718] + frame[719] + frame[720] + frame[721] + frame[722] + frame[723] + frame[724] + frame[725] + frame[747] + frame[748] + frame[749] + frame[750] + frame[751] + frame[752] + frame[753] + frame[754] + frame[755] + frame[756] + frame[757];
  right_heat  = frame[0] + frame[1] + frame[2] + frame[3] + frame[4] + frame[5] + frame[6] + frame[7] + frame[8] + frame[9] + frame[10] + frame[32] + frame[33] + frame[34] + frame[35] + frame[36] + frame[37] + frame[38] + frame[39] + frame[40] + frame[41] + frame[42] + frame[64] + frame[65] + frame[66] + frame[67] + frame[68] + frame[69] + frame[70] + frame[71] + frame[72] + frame[73] + frame[74] + frame[96] + frame[97] + frame[98] + frame[99] + frame[100] + frame[101] + frame[102] + frame[103] + frame[104] + frame[105] + frame[106] + frame[128] + frame[129] + frame[130] + frame[131] + frame[132] + frame[133] + frame[134] + frame[135] + frame[136] + frame[137] + frame[138] + frame[160] + frame[161] + frame[162] + frame[163] + frame[164] + frame[165] + frame[166] + frame[167] + frame[168] + frame[169] + frame[170] + frame[192] + frame[193] + frame[194] + frame[195] + frame[196] + frame[197] + frame[198] + frame[199] + frame[200] + frame[201] + frame[202] + frame[224] + frame[225] + frame[226] + frame[227] + frame[228] + frame[229] + frame[230] + frame[231] + frame[232] + frame[233] + frame[234] + frame[256] + frame[257] + frame[258] + frame[259] + frame[260] + frame[261] + frame[262] + frame[263] + frame[264] + frame[265] + frame[266] + frame[288] + frame[289] + frame[290] + frame[291] + frame[292] + frame[293] + frame[294] + frame[295] + frame[296] + frame[297] + frame[298] + frame[320] + frame[321] + frame[322] + frame[323] + frame[324] + frame[325] + frame[326] + frame[327] + frame[328] + frame[329] + frame[330] + frame[352] + frame[353] + frame[354] + frame[355] + frame[356] + frame[357] + frame[358] + frame[359] + frame[360] + frame[361] + frame[362] + frame[384] + frame[385] + frame[386] + frame[387] + frame[388] + frame[389] + frame[390] + frame[391] + frame[392] + frame[393] + frame[394] + frame[416] + frame[417] + frame[418] + frame[419] + frame[420] + frame[421] + frame[422] + frame[423] + frame[424] + frame[425] + frame[426] + frame[448] + frame[449] + frame[450] + frame[451] + frame[452] + frame[453] + frame[454] + frame[455] + frame[456] + frame[457] + frame[458] + frame[480] + frame[481] + frame[482] + frame[483] + frame[484] + frame[485] + frame[486] + frame[487] + frame[488] + frame[489] + frame[490] + frame[512] + frame[513] + frame[514] + frame[515] + frame[516] + frame[517] + frame[518] + frame[519] + frame[520] + frame[521] + frame[522] + frame[544] + frame[545] + frame[546] + frame[547] + frame[548] + frame[549] + frame[550] + frame[551] + frame[552] + frame[553] + frame[554] + frame[576] + frame[577] + frame[578] + frame[579] + frame[580] + frame[581] + frame[582] + frame[583] + frame[584] + frame[585] + frame[586] + frame[608] + frame[609] + frame[610] + frame[611] + frame[612] + frame[613] + frame[614] + frame[615] + frame[616] + frame[617] + frame[618] + frame[640] + frame[641] + frame[642] + frame[643] + frame[644] + frame[645] + frame[646] + frame[647] + frame[648] + frame[649] + frame[650] + frame[672] + frame[673] + frame[674] + frame[675] + frame[676] + frame[677] + frame[678] + frame[679] + frame[680] + frame[681] + frame[682] + frame[704] + frame[705] + frame[706] + frame[707] + frame[708] + frame[709] + frame[710] + frame[711] + frame[712] + frame[713] + frame[714] + frame[736] + frame[737] + frame[738] + frame[739] + frame[740] + frame[741] + frame[742] + frame[743] + frame[744] + frame[745] + frame[746];


  if(left_heat >= center_heat and left_heat >= right_heat) {
    motor_adjustment=calc_new_position(motor_adjustment+SERVO_TURN_LEFT);  
    set_led(LEFT_EYE,0,255,0);
    set_led(RIGHT_EYE,0,255,0);
    Serial.print( "left_heat:" );
  }
  else if(right_heat >= center_heat and right_heat >= left_heat) {
    motor_adjustment=calc_new_position(motor_adjustment+SERVO_TURN_RIGHT); 
    set_led(LEFT_EYE,0,0,255);
    set_led(RIGHT_EYE,0,0,255);
    Serial.print( "right_heat:" );
  }
  else {
    motor_adjustment=calc_new_position(motor_adjustment);
    set_led(LEFT_EYE,255,0,0);
    set_led(RIGHT_EYE,255,0,0);
    Serial.print( "center_heat:" );
  }
  
  Serial.println( motor_adjustment );
  pwm.setPWM(SERVONUM, 0, motor_adjustment);

}
