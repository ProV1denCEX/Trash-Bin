
% datenum/datestr to yyyymmdd
% 38s
tic
for i = 1 : 5000
    aaa = num2cell(str2double(datestr(cDate, 'yyyymmdd')))
end
toc
