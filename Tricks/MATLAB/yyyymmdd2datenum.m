% Slower
tic
for i = 1 : 1000
    dTime = dTimeAxis(:, 1);
    dTime = datenum(num2str(dTime), 'yyyymmdd');
end
toc

% Faster
tic
for i = 1 : 1000
    dTime = dTimeAxis(:, 1);
    dTime(:, 2) = floor(dTime(:, 1) / 10000);
    dTime(:, 3) = floor((dTime(:, 1) - dTime(:, 2) * 10000) / 100);
    dTime(:, 4) = mod(dTime(:, 1), 10);
    dTime(1, 7) = 0;
    dTime = datenum(dTime(:, 2 : end));
end
toc