%% Decision Tree Trainer
%
% * author: Erikson Sodergren
%
% end section 

%% Initialization
%prepare files to read/write
filename = fullfile('trainData.csv');
T = readtable(filename);
fileid=fopen('hw05_Sodergren_Erikson_Classifier.m', 'w');
% end section

%% emit_main
%
% write main section of the classifier
fprintf(fileid, 'filename = fullfile(''validData.csv'');\n');
fprintf(fileid, 'T = readtable(filename);\n');
fprintf(fileid, 'T.Guess = zeros(height(T), 1);\n');
fprintf(fileid, 'for i=1:height(T)\n');
fprintf(fileid, '\tT(i, :) = decTree(T(i, :));\n');
fprintf(fileid, 'end\n');
fprintf(fileid, 'T = [T(:, 1) T(:, 27) T(:, 2:26)]; \n');
fprintf(fileid, 'writetable(T, ''hw05_Sodergren_Erikson_MyClassifications.csv'');\n');
fprintf(fileid, 'writetable(T(:, 2), ''hw05_Sodergren_Erikson_1lineClassifications.csv'');\n');
fprintf(fileid, ' \n');
fprintf(fileid, ' \n');
fprintf(fileid, 'function tableLine = decTree(tableLine)\n');
emit_decTree(T, fileid, 1);%print decision tree itself
fprintf(fileid, 'end\n');
% end section

%% emit_decTree
%
% create the decision tree function
function emit_decTree(subset, fileid, numTabs)
    
    
    if(height(subset)<=5 || calcGini(subset)<0.10)%stop condition
        maj=majority(subset);
        tab(numTabs, fileid);
       fprintf(fileid, 'tableLine.Guess = %i;\n', maj);%0=cupcake,1=muffin
    else
        bestAttr='';
        bestGood=9;
        bestThresh=0;
        variables=subset.Properties.VariableNames; %get variable names
        for i=variables(2:width(subset))%test each attribute
            i
            thisThresh=0;
            thisGood=9;
            for thresh=1:100%test each possible threshold
                left=subset(subset.(char(i))<=thresh, :);
                right=subset(subset.(char(i))>thresh, :);
                good=weightGini(left, right);
                if(good<thisGood)
                   thisGood=good;
                   thisThresh=thresh;
                end
            end
            if(thisGood<bestGood)
               bestAttr=char(i);
               bestGood=thisGood;
               bestThresh=thisThresh;
            end
        end
        left=subset(subset.(bestAttr)<=bestThresh, :);%recurse
        right=subset(subset.(bestAttr)>bestThresh, :);
        tab(numTabs, fileid);
        fprintf(fileid, 'if(tableLine.%s <= %i)\n', bestAttr, bestThresh);
        emit_decTree(left, fileid, numTabs+1);
        tab(numTabs, fileid);
        fprintf(fileid, 'else\n');
        emit_decTree(right, fileid, numTabs+1);
        tab(numTabs, fileid);
        fprintf(fileid, 'end\n');
    end
end

function tab(numTabs, fileid)%print tabs for making the nested if's readable
    if(numTabs>0)
       for i=1:numTabs
           fprintf(fileid, '\t');
       end
    end
end

function wgini = weightGini(left, right)%calculate weighted gini
    leftTotal=height(left);
    rightTotal=height(right);
    total=leftTotal+rightTotal;
    wgini=((leftTotal/total)*calcGini(left))+((rightTotal/total)*calcGini(right));
    
end

function gini = calcGini(subset)%calculate individual gini
    count = [0,0];
    total=height(subset);
    for i=1:total
        temp=char(subset(i,:).Type);
        if(strcmp(temp, 'Muffin'))
           count(1)=count(1)+1;
        else
           count(2)=count(2)+1;
        end
    end
    gini = 1-(((count(1)/total)^2)+((count(2)/total)^2));
    if(count(1)==0||count(2)==0)
       gini=0; 
    end
end

function majType = majority(subset)%find majority type in the subset
    count = [0,0];
    for i=1:height(subset)
        temp=char(subset(i,:).Type);
        if(strcmp(temp, 'Muffin'))
           count(1)=count(1)+1;
        else
           count(2)=count(2)+1;
        end
    end
    if(count(1)>=count(2))
        majType= 1;%1==muffin
    else
        majType= 0;%0==cupcake
    end
end
%fprintf(fileid, 'tableLine.type2=1;\n');
% end section