/*
 CSci-4611 Assignment #1 Text Rain
*/


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;
boolean debug = false;  //later will be used in the debugging mode
boolean greyMode = false;  //grey mode
int inputImageWidth;
PFont  font;
ArrayList<Letter> letters;  //set a array that will store everything
String text = "This is my TextRain Assignment, enjoy the feeling of rain falling on your face and have a good one, the falling text will land on anything darker than a certain threshord and fall whenever that obstacle removed.";
int save_time ; //this will record the time when the function was last recalled.
float threshould = 0.4;
//PImage img;
void setup() {
 size(1280, 720);
 inputImage = new PImage(width, height, RGB);
 font = loadFont("Arial-BoldItalicMT-15.vlw");   //set the font that I want to use
 textFont(font);
 letters = new ArrayList();
}
class Letter{
 int x;
 int y; //set up the x y corronidnate

 int speed;  //set up the velocity of the falling letters
 int size;
 color c;  //color of the letters
 PFont fo =loadFont("Arial-BoldItalicMT-15.vlw");   //set the font that I wanted
 char textLetter;
 Letter() {   //constrcutor
 y = 40;
 int tempx = (int) random(0, 1280);
 x = tempx;
 textLetter = (char)text.charAt(int(random(0,50)));
 speed = int((random(1, 6)));   //try to make the falling speed different
 c = color(255, int(random(100,255)), 255);  //make the random colcor
 int time_measurement;
 textFont(fo);
 }
 void change_size(PImage new_image)
 {
   boolean ispointdark =false;  //to determine if a pixel point is dark or not
   boolean raiseletter = false; //whether to raise the letter

    //if the falling letter hit anything grey  then the stop case kicks in

 //I have to compare the falling letter to the pixel point that it has pass
 color points =new_image.pixels[(x)+ new_image.width * (y)];
 if(( points==color(0,0,0)))
 {
   if(points==color(0,0,0)){
     raiseletter = true;

   }
 }
 if ((points != color(0,0,0)))
 {
  y = drop_case(y);
  ispointdark = false;
 }else{

   ispointdark = true;
 }
 if (raiseletter == true)   //the case that it hit a dark spot and need to bounce back with the movement of the dark spot
 {
    y = updrop(y);
 ispointdark = true;
 }

 }

}

void draw() {
 // When the program first starts, draw a menu of different options for which camera to use for input
 // The input method is selected by pressing a key 0-9 on the keyboard

 if (!inputMethodSelected) {
 cameras = Capture.list();
 int y=40;
 text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
 y += 40;
 for (int i = 0; i < min(9,cameras.length); i++) {
 text(i+1 + ": " + cameras[i], 20, y);
 y += 40;
 }
 return;
 }


 // This part of the draw loop gets called after the input selection screen, during normal execution of the program.


 // STEP 1. Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable

if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }

  // This code draws the current camera/movie image to the screen
  set(0, 0, inputImage);


 // Fill in your code to implement the rest of TextRain here..

 // Tip: This code draws the current input image to the screen

 save_time = millis();  // the millis function is related with real time and it helps control the velocity of the dropping letter
 PImage Img;
 PImage pic = new PImage(inputImage.width, inputImage.height, RGB);
 //the copy method not works so well so I just looped and assign all the value
 int h =inputImage.height;
 int w = inputImage.width;
 for (int j = 0; j < h; j++)
 for (int i = 0; i < w; i++) {
 pic.pixels[i + w * j] = inputImage.pixels[i + w * j];
 }
 //just copy the pixels to pic that I seted
 //pic.copy(mov,0,0,mov.height,0,0,inputImage.width, inputImage.height);
 //I tried to flipped the image but not success
  /*
  pushMatrix();
  scale(-1.0,1.0);
  image(inputImage,-inputImage.width,0);
  popMatrix;
  */

 Img = pic;
 Img.filter(THRESHOLD,threshould);

 if (debug==true) {
 inputImage = Img;
 }
 else if (greyMode == true){   //grey mode
 inputImage.filter(GRAY);
 }
 else{
   inputImage.filter(GRAY);
 }
 set(0, 0, inputImage);
 int tempW =Img.width;
 int tempH =Img.height;
 Letter a = new Letter(); //get a new char
 if (a.x + a.size < tempW)
 {
   if  (  a.y + a.size < tempH)
   {
         Letter letter = new Letter();
         letters.add(letter);
 }
 }
 int i =0;
 while(i<letters.size()){
 a = (Letter) letters.get(i);
 if (a.x >= 0 && a.x + a.size < tempW)
 {
   if (a.y >= 0 && a.y + a.size < tempH)
    {
 a.change_size(Img);
 PFont fon = loadFont("CenturyGothic-Bold-20.vlw"); // new font will be applied to those letter we encounter dark point
 textFont(fon);
 a.c = color(int(random(0,250)),int(random(0,250)),int(random(0,250)));  //given a random color
 fill(a.c);  //fill will a random color
 text(a.textLetter, a.x, a.y);
 }
 }
 i+=1;
 }
}


void keyPressed() {

 if (!inputMethodSelected) {
 // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
 if ((key >= '0') && (key <= '9')) {
 int input = key - '0';
 if (input == 0) {
 println("Offline mode selected.");
 mov = new Movie(this, "TextRainInput.mov");
 mov.loop();
 inputMethodSelected = true;
 }
 else if ((input >= 1) && (input <= 9)) {
 println("Camera " + input + " selected.");
 // The camera can be initialized directly using an element from the array returned by list():
 cam = new Capture(this, cameras[input-1]);
 cam.start();
 inputMethodSelected = true;
 }
 }
 return;
 }


 // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
 // Fill in your code to handle keypresses here..

 if (key == CODED) {
 if (keyCode == UP) {
 // up arrow key pressed
 threshould += 0.1;
 }
 else if (keyCode == DOWN) {
 // down arrow key pressed
 threshould -= 0.1;
 }
 }
 else if (key == ' ') {
 // space bar pressed
 debug = !debug;
 }
 else if(key == 'g'){   //grey mode option was set
   greyMode = true;
 }
}



int drop_case(int y)  //the case that the text letter drop with a random velocity
{
  y+=int((random(3, 6)));
  return y;
}
int updrop(int x)  // the case that the letter bou
{
  if (x>0){
  x-=10;
  }
  return x;
}
