%% morse vowels
%
% * author: Erikson Sodergren
%
% end section 

%% Initialization
%
% If there are numbers that you need to define up front, or that you want
% easily accessible, define them in this section.
fileIn = fopen('morse.txt', 'r');
input = fscanf(fileIn, '%s');
tempstr = '';
templen=3;%used to find what a '-' is for, max length of 3
length=strlength(input);
pos=ones(1,strlength(input)+1);%index length+1 is for mathing
% end section

%% 
%
% Describe what MATLAB will do in this section, then type commands below
%result = countMorse(1, tempstr);
for i=length:-1:1
    if(strcmp(tempstr, ''))%first char of a letter
        if(strcmp(input(i), '-'))
           pos(i)=0;%just one dash isnt valid
           tempstr(templen)=input(i);
           templen=templen-1;
        else%its a dot
            if(i==length)
               pos(i)=1;
            else
               pos(i)=pos(i+1);
            end
            tempstr(templen)=input(i);
            templen=templen-1;
        end
    else%not first char of a letter
        if(strcmp(input(i), '-'))%dash after something
            if(strcmp(input(i+1), '.'))%dot previosuly
                pos(i)=0;
                tempstr='  -';
                templen=2;
            else%dash previosuly
                templen=templen-1;
                if(templen==0)%it's an O
                   pos(i)=pos(i+3);
                   tempstr='';
                   templen=3;
                else
                   pos(i)=0;
                   tempst=' --';
                end
            end
        else%dot after something
            if(strcmp(input(i+1), '.'))%extra dot
                pos(i)=pos(i+1)+pos(i+2);%possibilities of a string of dots follows fibonachi sequence
            else
                tempstr='';
                pos(i)=pos(i+(4-templen));%same as before this letter began
                templen=3;
            end
        end
    end
end
fprintf('this morse code can be read as only vowels in %i ways\n', pos(1))
% end section