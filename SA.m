% Simulated Annealing Algorithm (script)


% ideas from
% http://katrinaeg.com/simulated-annealing.html
% http://en.wikipedia.org/wiki/Simulated_annealing
% http://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6

function [bx,by,bncc] = SA

I1 = double(imread('img1_sm.tif'));
I2 = double(imread('img2_sm.tif'));

% I1 = double(imread('img1.tif'));
% I2 = double(imread('img2.tif'));

% get image size
[m,n] = size(I1);
% valid translations from I1 to I2 are [-(m-1):(m-1),-(n-1):(n-1)]

% create initial valid translation
x = round(n*rand()); % rand = [0-1]
y = round(m*rand());
[x,y] = bound(x,y,m,n);

% compute ncc for init solution
ncc = get_cost(I1,I2,x,y);
temperature = 1000.0;
alpha = 0.999;
itr_count = 0;

bx = x; by = y; bncc = ncc;

% loop over temperature cooling
while temperature > 0.00001
  
  % update temperature
  temperature = temperature * alpha;
  if temperature < eps('single')
    fprintf('min temp reached\n');
    break;
  end
  
  itr_count = itr_count + 1;
  
  % first check the 4 neighbors (aka perform normal hill climb
  [new_x,new_y,new_ncc] = perform_hc_step(I1,I2,x,y,ncc,m,n);
  if new_x ~= x || new_y ~= y
    % accept this move
    ncc = new_ncc;
    x = new_x;
    y = new_y;
    
    if ncc > bncc
      bx = x;by = y; bncc = ncc;
    end
    
    continue;
  end
  % end neighbors check
  
  
  % generate a random new location to hill climb from
  [new_x,new_y] = generate_neighbor(m,n);
  
  % compute new cost
  new_ncc = get_cost(I1,I2,new_x,new_y);
  
  if(new_ncc > ncc)
    % accept this move
    ncc = new_ncc;
    x = new_x;
    y = new_y;
    
    if ncc > bncc
      bx = x;by = y; bncc = ncc;
    end
    
  else
    % maybe accept new worse solution
    energy = exp(abs(ncc - new_ncc) / temperature);
    
    if energy > rand()
      % accept new worse solution
      ncc = new_ncc;
      x = new_x;
      y = new_y;
      
      if ncc > bncc
      bx = x;by = y; bncc = ncc;
    end
      
    else
      % reject new solution
      % no updates to x,y, or ncc
    end
  end
  
  
  
end

fprintf('best: (x,y,c) = (%d,%d,%g)\n',bx,by,bncc);

% fprintf('(x,y,ncc) = (%d,%d,%g)\n',x,y,ncc);
fprintf('%d Iterations\n',itr_count);

end


function [x,y,ncc] = perform_hc_step(I1,I2,rx,ry,rncc,m,n)

x_vals = zeros(4,1);
y_vals = zeros(4,1);
ncc_vals = zeros(4,1);



% check left
x = rx-1;
y = ry;

[x,y] = bound(x,y,m,n);
ncc = get_cost(I1,I2,x,y);
x_vals(1) = x;
y_vals(1) = y; 
ncc_vals(1) = ncc;

% check right
x = rx+1;
y = ry;

[x,y] = bound(x,y,m,n);
ncc = get_cost(I1,I2,x,y);
x_vals(2) = x;
y_vals(2) = y; 
ncc_vals(2) = ncc;

% check top
x = rx;
y = ry-1;

[x,y] = bound(x,y,m,n);
ncc = get_cost(I1,I2,x,y);
x_vals(3) = x;
y_vals(3) = y; 
ncc_vals(3) = ncc;

% check bottom
x = rx;
y = ry+1;

[x,y] = bound(x,y,m,n);
ncc = get_cost(I1,I2,x,y);
x_vals(4) = x;
y_vals(4) = y; 
ncc_vals(4) = ncc;

[ncc,idx] = max(ncc_vals(:));

if ncc > rncc
  x = x_vals(idx);
y = y_vals(idx);
else
  x = rx; y = ry; ncc = rncc;
end


end


function ncc = get_cost(I1,I2,x,y)

% ncc exists in [-1, 1]
ncc = compute_ncc(I1,I2,x,y);

end

function [x,y] = generate_neighbor(m,n)


x = round((n-1)*rand());
if rand() > 0.5
 x = -x;
end

y = round((m-1)*rand());
if rand() > 0.5
  y = -y;
end

[x,y] = bound(x,y,m,n);

end


function [x,y] = bound(x,y,m,n)

x = max(x, -(n-1));
x = min(x, (n-1));

y = max(y, -(m-1));
y = min(y, (m-1));

end

