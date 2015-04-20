function [x,y,c] = pciam(I1,I2)

f1 = fft2(I1); % FFT of the first image
f2 = fft2(I2); % FFT of second image

% Perform phase correlation between F1 and F2 (amplitude is normalized)
fc = f1 .* conj(f2);
fc(fc == 0) = eps;
fcn = fc ./(abs(fc));

% Inverse fourier of peak correlation matrix
pcm = ifft2(fcn);

% find the max value in the pcm matrix
[~, idx] = max(pcm(:));
% convert linear index to x,y
[y,x] = ind2sub(size(pcm), idx);
y = y - 1;
x = x - 1;

c = cross_correlation_matrix(extract_subregion(I1, x, y), extract_subregion(I2, -x, -y));


%x = col of max value
%y = row of max value
%c = max value

end


function cr = cross_correlation_matrix(I1, I2)

% Transform inputs into one column vectors
I1 = I1(:);     I2 = I2(:);
nb_elem = size(I1,1);

% center vectors to the mean
mv = sum(I1)/nb_elem;
I1 = I1 - mv;
mv = sum(I2)/nb_elem;
I2 = I2 - mv;
% I1 = I1-mean(I1);       I2 = I2-mean(I2);

% Compute the Numerator and Denominator of the cross-correlation formula
N = I1'*I2;
D = sqrt(I1'*I1) * sqrt(I2'*I2);

% Compute the cross-correlation factor
cr = N/D;

if ~isfinite(cr)
  cr = -1;
end

end


function [sub_I] = extract_subregion(I, x, y)
[h, w] = size(I);
if abs(x) >= w || abs(y) >= h
  sub_I = [];
  return;
end

x_st = 1 + x;
x_end = x_st + w - 1;
y_st = 1 + y;
y_end = y_st + h - 1;
% constrain to valid coords
x_st = max(1, min(x_st, w));
x_end = max(1, min(x_end, w));
y_st = max(1, min(y_st, h));
y_end = max(1, min(y_end, h));

sub_I = I(y_st:y_end, x_st:x_end);
end
