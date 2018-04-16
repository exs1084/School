%% longest increasing substring
%
% * author: Erikson Sodergren
%
% end section 

%% Initialization
%
% If there are numbers that you need to define up front, or that you want
% easily accessible, define them in this section.
fileid = fopen('input.txt', 'r');
global input;
input = fscanf(fileid, '%i');
global sols;
sols = zeros(size(input, 1), 1);
%sols is a table of solutions where sols(i) contains the solution of length
%i that ends in the smallest number possible so far.
output(1)=0;
output(2)=input(1);
sols(1)=input(1);
currBest = 1;
% end section

%% Type the title of this section here
%
for i=2:size(input, 1)%put each new nubmer into the function
    newBest = longInSeq(i, currBest);
    currBest = max(newBest, currBest);
end
fprintf('the longest increasing subsequence is: ');%print asnwer
for i=1:currBest
   fprintf('%i ', sols(currBest, i)); 
end
fprintf('\n');
% end section


%function longInSeq takes in the index of the current number to place,
%and the current solution length to try and append it to. if that solution
%would become invalid, the nest smallest solution is attempted.
function newBest = longInSeq(i, currLen)
    global sols;
    global input;
    if(currLen==0)
       sols(1) = input(i);
       newBest = 1;
    else
        if(input(i)>=sols(currLen, currLen))
            sols(currLen+1, :) = sols(currLen, :);
            sols(currLen+1, currLen+1) = input(i);
            newBest = currLen+1;
        else
            newBest = longInSeq(i, currLen-1);
        end
    end
end