% Odczytaj plik CSV
positions = readtable('positions_ParticleSim.csv');

% Display a few rows
disp(positions([1 end], :));

% Plot the particle's all positions
scatter(positions.y1_m_, positions.y2_m_, 2, positions.time_s_, 'filled');
%plot(positions.time_s_,positions.y1_m_);
grid on; hold on;
clear all