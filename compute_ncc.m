function ncc = compute_ncc(I1,I2,x,y)


ncc = cross_correlation_matrix(extract_subregion(I1, x, y), extract_subregion(I2, -x, -y));

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
