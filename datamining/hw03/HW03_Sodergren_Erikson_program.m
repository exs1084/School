%% 1D classification
%
% * author: Erikson Sodergren
% end section 

%% Initialization
%
%import the data file as numeric array, with name "data" beforehand
min=99999;
max=0;
bins=zeros(99, 2);
for k=1:size(data, 1)%count how many drivers at each(rounded)speed were speeding intentionally or not
   temp=round(data(k, 1));
   temp2=data(k, 2);
   if(temp<min)
       min=temp;
   elseif(temp>max)
       max=temp;
   end
   if(temp2==0)
       bins(temp, 1)=bins(temp, 1)+1;%first column of bins is number of non-speeders
   else
       bins(temp, 2)=bins(temp, 2)+1;%second column is speeders
   end
end
trueNeg=zeros(max,1);
truePos=zeros(max,1);
falseNeg=zeros(max,1);
falsePos=zeros(max,1);
leastFalse=99999;
bestSpeed=0;
% end section

%% work
for k=min:max%try at every possible speed value we have any drivers
    for j=min:k%%ignored
        trueNeg(k)=trueNeg(k)+bins(j, 1);
        falseNeg(k)=falseNeg(k)+bins(j, 2);
    end
    for j=k+1:max%%caught
        truePos(k)=truePos(k)+bins(j, 2);
        falsePos(k)=falsePos(k)+bins(j, 1);
    end
    misclass(k)=falseNeg(k)+falsePos(k);
    if((misclass(k))<=leastFalse)
       leastFalse= misclass(k);
       bestSpeed=k;
    end
end

% end section

%% graphing

hold on;
title('1D classifying for speeding cars');
ylabel('misclassification rate')
xlabel('Threshold')
plot(min:max, misclass(min:max));
plot(bestSpeed, leastFalse, 'r*');
hold off;

figure;
hold on;
title('ROC curve');
ylabel('True Positive')
xlabel('False Positive')
plot(falsePos(min:max), truePos(min:max));
plot(falsePos(bestSpeed), truePos(bestSpeed), 'ro');
hold off;

% end section