%matlab_script

I1 = imread('img1_sm.tif');
I2 = imread('img2_sm.tif');

I1 = double(I1);
I2 = double(I2);

[x,y,c] = pciam(I1,I2);
