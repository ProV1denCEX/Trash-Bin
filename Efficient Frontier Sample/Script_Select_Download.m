clear
clc

rng(0)

%% 0. Data
% 0.1 Stock
[dRaw, ~, cRaw] = xlsread('ListAndPrice.csv');

dList = unique(dRaw(:, 1));
dSelect = zeros(length(dList), 2);

for icode = 1 : length(dList)
    dLocated = dRaw(:, 1) == dList(icode);
    dTemp = dRaw(dLocated, :);
    
    if (dTemp(1, 2) <= 20080131 && dTemp(end, 2) >= 20171231) &&...
            all(all(~isnan(dTemp(:, 14 : 16)))) && length(dTemp) >= 120
        dSelect(icode, 1) = dList(icode);
        dSelect(icode, 2) = dTemp(end, 9);
    else
    end
    
end

dLocated = dSelect(:, 1) == 0;
dSelect(dLocated, :) = [];
dLocated = isnan(dSelect(:, 2));
dSelect(dLocated, :) = [];

dIndustry = unique(dSelect(:, 2));
dSelected_Indsutry = floor(rand(10, 1) * length(dIndustry));
dSelected_Indsutry = dIndustry(dSelected_Indsutry);

dSelected = zeros(10, 1);
for iInd = 1 : length(dSelected_Indsutry)
    iIndustry = dSelected_Indsutry(iInd);
    dLocated = dSelect(:, 2) == iIndustry;
    dTemp = dSelect(dLocated, 1);
    
    dLocated = ceil(rand(1) * length(dTemp));
    dSelected(iInd) = dTemp(dLocated);
    
end

cStock = struct();
dCalendar = unique(dRaw(:, 2));
dLocated = dCalendar(:, 1) >= 20080131 & dCalendar(:, 1) <= 20171231;
dCalendar = dCalendar(dLocated);
for iStock = 1 : length(dSelected)
    dLocated = dRaw(:, 1) == dSelected(iStock);
    dTemp = dRaw(dLocated, :);
    
    cStock(iStock).PermNo = dTemp(1);
    
    dLocated_c = [0; dLocated];
    cTemp = cRaw(logical(dLocated_c), :);
    cStock(iStock).Code = cTemp{1, 8};
    cStock(iStock).Name = cTemp{1, 6}; 
    
    dLocated = dTemp(:, 2) >= 20080131 & dTemp(:, 2) <= 20171231;
    dTemp = dTemp(dLocated, [2, 14 : 16]);
    [~, dLocated_in, dLocated_out] = intersect(dCalendar, dTemp(:, 1));
    dTemp = dTemp(dLocated_out, 2 : end);
    dTemp(:, end) = dTemp(:, 2) ./ dTemp(:, 3);
    cStock(iStock).Data = [dCalendar, dTemp];
    cStock(iStock).Data(:, end + 1) = [nan; diff(cStock(iStock).Data(:, end)) ./ cStock(iStock).Data(1 : end - 1, end)];
end

% 0.2 Index
cIndex = struct();
[dRaw, ~, ~] = xlsread('NASDAQ.csv');
cIndex.Code = 'NASDAQ';
cIndex.Name = 'NASDAQ';
dLocated = dRaw(:, 1) >= 20080131 & dRaw(:, 1) <= 20171231;
cIndex.Data = dRaw(dLocated, 1 : 2);

dRaw = xlsread('SP500.csv');
cIndex(end + 1).Code = 'SP500';
cIndex(end).Name = 'SP500';
dLocated = dRaw(:, 1) >= 20080131 & dRaw(:, 1) <= 20171231;
cIndex(end).Data = dRaw(dLocated, 1 : 2);

dRaw = xlsread('T_Bill.csv');
cIndex(end + 1).Code = 'T_Bill';
cIndex(end).Name = 'T_Bill';
dLocated = dRaw(:, 1) >= 20080131 & dRaw(:, 1) <= 20171231;
cIndex(end).Data = dRaw(dLocated, 1 : 2);

%% 1. Question 1
% 1.1 mean & std estimate
dReturn = zeros(length(cStock(1).Data), length(cStock));
for iStock = 1 : length(cStock)
    dReturn(:, iStock) = cStock(iStock).Data(:, end);
end
dMean = nanmean(dReturn);
dCov = nancov(dReturn);
cName = {cStock.Name};

% 1.2 Portfolio construction
cPortfolio = Portfolio('AssetMean', dMean, 'AssetCovar', dCov, 'AssetList', cName);
cPortfolio = setDefaultConstraints(cPortfolio); cPortfolio.LowerBound(:) = -1; cPortfolio.UpperBound(:) = 1;
cPortfolio.LowerBound(:) = -1;
cPortfolio.UpperBound(:) = 1;
plotFrontier(cPortfolio);
saveas(gcf, 'Question1.jpg')

%% 2. Question 2
nMean_SP = mean(cIndex(2).Data(:, 2));
nStd_SP = std(cIndex(2).Data(:, 2));
nMean_NAS = mean(cIndex(1).Data(:, 2));
nStd_NAS = std(cIndex(1).Data(:, 2));

cPortfolio = Portfolio('AssetMean', dMean, 'AssetCovar', dCov, 'AssetList', cName);
cPortfolio = setDefaultConstraints(cPortfolio); cPortfolio.LowerBound(:) = -1; cPortfolio.UpperBound(:) = 1;
dWeight = estimateFrontier(cPortfolio);
[dStd_Portfolio, dReturn_Portfolio] = estimatePortMoments(cPortfolio,dWeight);

portfolioexamples_plot('Asset Risks and Returns', ...
    {'line', dStd_Portfolio, dReturn_Portfolio}, ...
	{'scatter', nStd_SP, nMean_SP, {'SP500'}}, ...
    {'scatter', nStd_NAS, nMean_NAS, {'NASDAQ'}}, ...
	{'scatter', sqrt(diag(cPortfolio.AssetCovar)), cPortfolio.AssetMean, cPortfolio.AssetList, '.r'});
saveas(gcf, 'Question2.jpg')

%% 3. Question 3
cPortfolio_mvp = Portfolio('AssetMean', dMean, 'AssetCovar', dCov, 'AssetList', cName);
cPortfolio_mvp = setDefaultConstraints(cPortfolio_mvp); cPortfolio_mvp.LowerBound(:) = -1; cPortfolio_mvp.UpperBound(:) = 1;
dWeight_mvp = estimateFrontierByRisk(cPortfolio_mvp, 0);
[dStd_Portfolio_mvp, dReturn_Portfolio_mvp] = estimatePortMoments(cPortfolio_mvp, dWeight_mvp);

cPortfolio_mvp = setInitPort(cPortfolio_mvp,1/cPortfolio_mvp.NumAssets);
[dStd_Portfolio_ewp, dReturn_Portfolio_ewp] = estimatePortMoments(cPortfolio_mvp, cPortfolio_mvp.InitPort);

dWeight_ew = ones(10, 1) ./ 10;
dCov_mvp_ew = nancov(sum(dReturn .* dWeight_mvp', 2), sum(dReturn .* dWeight_ew', 2));
nIsEqual = dCov_mvp_ew(2) - dStd_Portfolio_mvp ^ 2 <= 10^-6;

%% 4. Question 4
cPortfolio = Portfolio('AssetMean', dMean, 'AssetCovar', dCov, 'AssetList', cName);
cPortfolio = setDefaultConstraints(cPortfolio); cPortfolio.LowerBound(:) = -1; cPortfolio.UpperBound(:) = 1;
dWeight = estimateFrontierByReturn(cPortfolio, dReturn_Portfolio_ewp);
[dStd_Portfolio, dReturn_Portfolio] = estimatePortMoments(cPortfolio, dWeight);

nAdjusted_Return = dReturn_Portfolio_ewp + 0.005;
dWeight = estimateFrontierByReturn(cPortfolio, nAdjusted_Return);
[dStd_Portfolio_Adjusted, dReturn_Portfolio_Adjusted] = estimatePortMoments(cPortfolio, dWeight);
dStd_Portfolio_Adjusted - dStd_Portfolio
% worth - >+, std

%% 5. Question 5
dReturn_const = dReturn(1 : 60, :);
dReturn_Test = dReturn(61 : end, :);
dMean_const = nanmean(dReturn_const);
dCov_const = nancov(dReturn_const);

cPortfolio = Portfolio('AssetMean', dMean_const, 'AssetCovar', dCov_const, 'AssetList', cName);
cPortfolio = setDefaultConstraints(cPortfolio); cPortfolio.LowerBound(:) = -1; cPortfolio.UpperBound(:) = 1;
cPortfolio = setInitPort(cPortfolio,1/cPortfolio.NumAssets);
[nStd_Portfolio_ewp, nReturn_Portfolio_ewp] = estimatePortMoments(cPortfolio, cPortfolio.InitPort);

dWeight = estimateFrontierByReturn(cPortfolio, nReturn_Portfolio_ewp + 0.005);
mean(sum(dReturn_Test .* dWeight', 2)) - mean(sum(dReturn_Test .* dWeight_ew', 2)) >= 0.005

%% 6. Question 6
nRF = mean(cIndex(end).Data(:, 2));
cPortfolio_rf = Portfolio('AssetMean', dMean, 'AssetCovar', dCov, 'AssetList', cName, 'RiskFreeRate', nRF);
cPortfolio_rf = setDefaultConstraints(cPortfolio_rf); cPortfolio_rf.LowerBound(:) = -1; cPortfolio_rf.UpperBound(:) = 1;
[dRsk, dRet_rf] = estimatePortMoments(cPortfolio_rf,estimateFrontier(cPortfolio_rf));

cPorfolio_Cash = setBudget(cPortfolio_rf, 0, 1);
dWeight_Cash = estimateFrontier(cPorfolio_Cash);
[dStd_rf, dReturn_rf] = estimatePortMoments(cPorfolio_Cash,dWeight_Cash);

portfolioexamples_plot('Efficient Frontier with Tangent Line', ...
	{'line', dRsk, dRet_rf}, ...
	{'line', dStd_rf, dReturn_rf, [], 'r', 1}, ...
    {'scatter', nStd_SP, nMean_SP, {'SP500'}}, ...
    {'scatter', nStd_NAS, nMean_NAS, {'NASDAQ'}}, ...
	{'scatter', sqrt(diag(cPortfolio.AssetCovar)), cPortfolio.AssetMean, cPortfolio.AssetList, '.r'});
legend('EF', 'rf')

saveas(gcf, 'Question6.jpg')

%% 7. Question 7
% 9 Stock returns
global dReturn_9
global dReturn_1

dReturn_9 = dReturn;
dReturn_1 = dReturn_9(:, ceil(rand(1) * 10));
dReturn_9(:, ceil(rand(1) * 10)) = [];
dCov_9 = nancov(dReturn_9);

% Given Risk, Parity CVaR
sTarget = @(x) sqrt(x(:)' * dCov_9 * x(:));
dConstraint_A = [];
dConstraint_b = [];
sConstraint_Nonl = @Fun_Constraint_NonL;

% Check the Limit
dConstraint_X0 = ones(9, 1) / 9;
dConstraint_vlb = zeros(1, 9);
dConstraint_vub = ones(1, 9);
dConstraint_Aeq = ones(1, 9);
dConstraint_beq = 1;

% Optimization
dWeightAllocate = fmincon( ...
    sTarget, ...
    dConstraint_X0, ...
    dConstraint_A, dConstraint_b,...
    dConstraint_Aeq, dConstraint_beq, ...
    dConstraint_vlb, dConstraint_vub, ...
    sConstraint_Nonl);

%% 8. Question 8
cPortfolio_mvp = Portfolio('AssetMean', dMean, 'AssetCovar', dCov, 'AssetList', cName);
cPortfolio_mvp = setDefaultConstraints(cPortfolio_mvp); cPortfolio_mvp.LowerBound(:) = -1; cPortfolio_mvp.UpperBound(:) = 1;
dWeight_mvp = estimateFrontierByRisk(cPortfolio_mvp, 0);
[dStd_Portfolio_mvp, dReturn_Portfolio_mvp] = estimatePortMoments(cPortfolio_mvp, dWeight_mvp);

cPortfolio_mvp = setInitPort(cPortfolio_mvp,1/cPortfolio_mvp.NumAssets);
[dStd_Portfolio_ewp, dReturn_Portfolio_ewp] = estimatePortMoments(cPortfolio_mvp, cPortfolio_mvp.InitPort);

dWeight_ew = ones(10, 1) ./ 10;

dReturn_mvp = sum(dReturn .* dWeight_mvp', 2);
dReturn_EW = sum(dReturn .* dWeight_ew', 2);

nVaR_EW_Norm = Fun_Cal_VaR(nReturn_Portfolio_ewp, nStd_Portfolio_ewp, 0.01, 1, 'Norm', []);
nVaR_EW_T = Fun_Cal_VaR(nReturn_Portfolio_ewp, nStd_Portfolio_ewp, 0.01, 1, 'T', []);
nVaR_EW_BS = Fun_Cal_VaR(nReturn_Portfolio_ewp, nStd_Portfolio_ewp, 0.01, 1, 'Bootstrap', dReturn_EW);

nVaR_mvp_Norm = Fun_Cal_VaR(dReturn_Portfolio_mvp, dStd_Portfolio_mvp, 0.01, 1, 'Norm', []);
nVaR_mvp_T = Fun_Cal_VaR(dReturn_Portfolio_mvp, dStd_Portfolio_mvp, 0.01, 1, 'T', []);
nVaR_mvp_BS = Fun_Cal_VaR(dReturn_Portfolio_mvp, dStd_Portfolio_mvp, 0.01, 1, 'Bootstrap', dReturn_mvp);



function [dConstraint, dEq] = Fun_Constraint_NonL(dWeight)
global dReturn_9
global dReturn_1
dConstraint = nancov(sum(dReturn_9 .* dWeight(:)', 2), dReturn_1);
dConstraint = dConstraint(end, 1);

dEq = [];

end


function ValueAtRisk = Fun_Cal_VaR(PortReturn, PortRisk, RiskThreshold, PortValue, sDistribution, dReturn)
% Make sure all arguments are columns
PortReturn = PortReturn(:);
PortRisk = PortRisk(:);
RiskThreshold = RiskThreshold(:);
PortValue = PortValue(:);

%x = norminv(p,mu,sigma)
if strcmp(sDistribution, 'Norm')
    X = norminv(RiskThreshold, PortReturn, PortRisk); 
elseif strcmp(sDistribution, 'T')
    X = tinv(RiskThreshold, 3);
elseif strcmp(sDistribution, 'Bootstrap')
    dTemp = dReturn(ceil(rand(10000, 1) * length(dReturn)));
    X = prctile(dTemp, 1);
end

ValueAtRisk  = (-min(X,0) .* PortValue);
end
