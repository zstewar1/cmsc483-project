% Simulated Annealing Algorithm (script)


% ideas from
% http://katrinaeg.com/simulated-annealing.html
% http://en.wikipedia.org/wiki/Simulated_annealing
% http://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6

startTime = tic();
% I1 = double(imread('img1_sm.tif'));
% I2 = double(imread('img2_sm.tif'));
% I1 = double(imread('img1_sm.tif'));
% I2 = double(imread('img2_sm.tif'));
I1 = double(imread('img1_med.tif'));
I2 = double(imread('img2_med.tif'));


% get image size
[m,n] = size(I1);
% valid translations from I1 to I2 are [-(m-1):(m-1),-(n-1):(n-1)]

delta = 2;
tmp1 = -(m-delta):(m-delta);
tmp2 = -(n-delta):(n-delta);
ncc_mat = NaN(numel(tmp1),numel(tmp2));

for i = 1:numel(tmp1)
  for j = 1:numel(tmp2)
    y = tmp1(i);
    x = tmp2(j);
    
    ncc_mat(i,j) = compute_ncc(I1,I2,x,y);
    
  end
end

[c,idx] = max(ncc_mat(:));

[i,j] = ind2sub(size(ncc_mat),idx);


x = tmp2(j)
y = tmp1(i)
c


fprintf('took: %d\n',toc(startTime));

