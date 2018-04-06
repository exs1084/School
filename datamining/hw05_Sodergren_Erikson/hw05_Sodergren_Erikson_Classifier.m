filename = fullfile('validData.csv');
T = readtable(filename);
T.Guess = zeros(height(T), 1);
for i=1:height(T)
	T(i, :) = decTree(T(i, :));
end
T = [T(:, 1) T(:, 27) T(:, 2:26)]; 
writetable(T, 'hw05_Sodergren_Erikson_MyClassifications.csv');
writetable(T(:, 2), 'hw05_Sodergren_Erikson_1lineClassifications.csv');
 
 
function tableLine = decTree(tableLine)
	if(tableLine.Sugar <= 19)
		if(tableLine.Egg <= 12)
			if(tableLine.ButterOrMargarine <= 17)
				tableLine.Guess = 1;
			else
				tableLine.Guess = 0;
			end
		else
			tableLine.Guess = 0;
		end
	else
		if(tableLine.Egg <= 23)
			if(tableLine.CannedPumpkin_or_Fruit <= 1)
				tableLine.Guess = 0;
			else
				tableLine.Guess = 1;
			end
		else
			tableLine.Guess = 1;
		end
	end
end
