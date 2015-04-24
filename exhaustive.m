% Simulated Annealing Algorithm (script)


% ideas from
% http://katrinaeg.com/simulated-annealing.html
% http://en.wikipedia.org/wiki/Simulated_annealing
% http://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6


I1 = double(imread('img1_sm.tif'));
I2 = double(imread('img2_sm.tif'));

% get image size
[m,n] = size(I1);
% valid translations from I1 to I2 are [-(m-1):(m-1),-(n-1):(n-1)]

tmp1 = -(m-1):(m-1);
tmp2 = -(n-1):(n-1);
ncc_mat = NaN(numel(tmp1),numel(tmp2));

for i = 1:numel(tmp1)
  for j = 1:numel(tmp2)
    y = tmp1(i);
    x = tmp2(j);
    
    ncc_mat(i,j) = compute_ncc(I1,I2,x,y);
    
  end
end


