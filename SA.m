Simulated Annealing Algorithm

% ideas from
% http://katrinaeg.com/simulated-annealing.html
% http://en.wikipedia.org/wiki/Simulated_annealing
% http://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6



% get image size
[m,n] = size(I1);

% create initial valid translation
x = rand(); % [0-1]
y = rand();
x = x*n;
y = y*m;

% compute ncc for init solution
ncc = compute_ncc(I1,I2,x,y);

% loop over temperature cooling
for temp = 100000:-0.1:0
	
	% generate random neighbor solution (change x or y)
	if(rand() > 0.5)
		% change x
		if(rand() > 0.5)
			new_x = x + 1;
		else
			new_x = x - 1;
	else
		% change y
		if(rand() > 0.5)
			new_y = y + 1;
		else
			new_y = y - 1;
	end

	% compute new cost
	new_ncc = compute_ncc(I1,I2,new_x,new_y);

	if(new_ncc < ncc)
		% accept this move
		ncc = new_ncc;
		x = new_x;
		y = new_y;
	else
		% maybe accept new worse solution

% this energy acceptance function is untested I need to see what the function looks like for different values of ncc, new_ncc

		energy = exp((ncc - new_ncc) / temp);
		if energy > rand()
			% accept new worse solution
			ncc = new_ncc;
			x = new_x;
			y = new_y;
		else
			% reject new solution
			% no updates to x,y, or ncc
		end
	end
	
end
