
% datenum/datestr to yyyymmdd
% 38s
tic
for i = 1 : 5000
    aaa = num2cell(str2double(datestr(cDate, 'yyyymmdd')));
end
toc

% 7s
tic
for i = 1 : 5000
    cTemp = regexp(cDate, '\d', 'match');
    cTemp = num2cell(str2double(cellfun(@(x) [x{:}], cTemp, 'UniformOutput', false)));
    cTemp = cTemp';
end
toc