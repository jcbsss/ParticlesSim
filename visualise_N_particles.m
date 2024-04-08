clear all
% Odczytaj plik CSV
positions = readtable('positions_ParticleSim.csv');

% Display a few rows
disp(positions([1 end], :));

% Read the data from the positions table
time = positions.time;
y1 = table2array(positions(1:5:end, 2:2:end)); % Convert y1 table to array
y2 = table2array(positions(1:5:end, 3:2:end)); % Convert y2 table to array

% Plot the initial positions of particles
figure;
scatter(y1(1, :), y2(1, :), 10, 'filled');
xlabel('y1 [m]'); xlim([0 7]);
ylabel('y2 [m]'); ylim([0 7]);
title('Initial Positions of Particles');
grid on;
axis equal;

% Loop through time steps and update the plot dynamically
for i = 1:size(y1, 1)
    scatter(y1(i, :), y2(i, :), 10, 'filled');
    xlabel('y1 [m]'); xlim([0 7]);
    ylabel('y2 [m]'); ylim([0 7]);
    title(['Positions of Particles at t = ', num2str(time(i)), 's']);
    grid on;
    axis equal;
    pause(0.001); % Adjust the pause time as needed for desired animation speed
end

