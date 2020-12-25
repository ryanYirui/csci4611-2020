REDEME
Yirui Zhao
1. I used my own text in my code which is: "This is my RainText assignment, enjoy the feeling of rain falling on your face and have a good one,
the falling text will land on anything darker than a certain threshoud and fall whenver the obstacle removed"

2. fisrt I set a change_size method, which is at the end of my code. I set a dark point (0,0,0) and compare the dark point with the falling text at certain
Pixels at the designed x-y coordinator. if there's nothing block the text , the text will keep falling untill reach the bottom end of the
frame that set by professor and then fade away.  this method change the font that I selected and also change the size of the font.

3 the up and down key is set for different light situation that audience will face.

4. a grey mode will be trigled if 'g' is detected by the user input.

5. a debug mode will kicks in when user press space .

6 to dropping and rising of the letter , I assigned all the letters in a array with letters, and then use for loop to go through every index of char in the
given text. I set a save_time to millis(), which is the function that can change the dropping letter process with the control of real time that it
go through.
