%% Agglom Clustering
% * author: Erikson Sodergren
% end section 

%% Initialization
%
filename = fullfile('shopdata.csv');
T = readtable(filename);
dataArray = table2array(T);
smallmerged=zeros(1,height(T));

crossCorr=zeros(width(T),width(T));
avg=mean(dataArray);
dev=std(dataArray);

for i=1:width(T)%loop first attribute
    for j=i+1:width(T)%loop second attribute
        for k=1:height(T)%loop which element to check
            tempval=((dataArray(k,i)-avg(i))/dev(i))*((dataArray(k,j)-avg(j))/dev(j));
            crossCorr(i,j)=crossCorr(i,j)+tempval;
        end
        crossCorr(i,j)=crossCorr(i,j)/height(T);%divide crosscorr(i,j) by N
        crossCorr(j,i)=crossCorr(i,j);
    end
    crossCorr(i,i)=1;
end
%remove id, petfood
T.ID=[];
T.PetFood=[];

agglomArray=table2array(T);
numRecs=height(T);
numVars=width(T);
dists=zeros(numRecs, numRecs);%each clusters ditstance from each other cluster
clusters=zeros(1, numVars, numRecs);%clusters(j,p,i):i=cluster #, j=record in cluster, p=attribute in record
center=zeros(numRecs,numVars);%average of each attribute in each cluster
clustsize=ones(numRecs,1);


for i=1:numRecs
   clusters(1,:,i)=agglomArray(i,:);
   center(i,:)=agglomArray(i,:);
   dists(i,i)=0;
end
% end section

%% agglomerate
%
maxClust=numRecs;
loopcount=1;
groupsfound=0;
while(1)%loop merges
   if(clustsize(1)==numRecs)
       break;
   end
   %find distances
   smallest=99999999;
   smalli=-1;
   smallj=-1;
   for i=1:maxClust
      if clustsize(i)>0
          for j=i+1:maxClust
              if(clustsize(j)>0)
                 distance=calcDist(center(i,:), center(j,:));%calculate distance here
                 dists(i,j)=distance;
                 dists(j,i)=distance;
                 if distance<smallest%closest pair?
                    smallest=distance;
                    smalli=i;
                    smallj=j;
                 end
              end
          end
      end
   end
   
   %do merging and size updating and smallest merge recording
   lowindex=min(smalli, smallj);
   highindex=max(smalli, smallj);
   smallsize=min(clustsize(smalli), clustsize(smallj));
   lowsize=clustsize(lowindex);
   for i=1:clustsize(highindex)
      clusters(lowsize+i,:,lowindex)=clusters(i,:,highindex); 
   end
   
   %recalc center
   if(smallsize>25)%recording 3 groups for writeup
       if(groupsfound<1)
           group3 = clusters(1:clustsize(highindex),:,highindex);
           familyPurchases = clusters(1:lowsize,:,lowindex);
           groupsfound=2;
       else
          partyAnimals = clusters(1:clustsize(highindex),:,highindex);
          groupsfound=3;
       end
   end
   
   smallmerged(loopcount)=smallsize;
   clustsize(lowindex)=lowsize+clustsize(highindex);
   clustsize(highindex)=0;
   
   %also change maxclust if the highest number cluster was merged
   if(highindex==maxClust)
      for i=1:maxClust
         if(clustsize(i)>0)
            newMax=i; 
         end
      end
      maxClust=newMax;
   end
   %break;%for testing
   loopcount=loopcount+1;
end

%writeup specific work
group3(38,:)=mean(group3);
familyPurchases(38,:)=mean(familyPurchases);
partyAnimals(27,:)=mean(partyAnimals);

% end section
function distance = calcDist(center1, center2)
    diff = center1 - center2;
    squared = diff.^2;
    summed = sum(squared);
    distance = sqrt(summed);
end