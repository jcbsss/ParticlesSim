% Clear workspace
clear all

% Read positions data from CSV file
positions = readtable('positions_ParticleSim.csv');

% Display a few rows
%disp(positions([1 end], :));

% Read the data from the positions table
time = positions.time;
t_steps_jump = 3;
y1 = table2array(positions(1:t_steps_jump:end, 2:2:end)); % Convert y1 table to array
y2 = table2array(positions(1:t_steps_jump:end, 3:2:end)); % Convert y2 table to array

% Default plotting parameters
L1 = 2*pi;
L2 = 4*pi;
part_size = 5;
animation_name = 'animation_A2_W.5_U1.5_R.95.mp4';
frame_rate = 30;

% Plot the initial positions of particles
figure('Position', [0, 0, L1/pi*256, L2/pi*256]);
scatter(y1(1, :), y2(1, :), part_size, 'filled');
xlabel('y1 [m]'); xlim([0 L1]);
ylabel('y2 [m]'); ylim([0 L2]);
daspect([L1 L2 1]);
title('Initial Positions of Particles');
grid on;
axis equal;

% Choose the action: play, save, or save and play
choice = input('Choose action: (1) Play animation, (2) Save animation: ');

switch choice
    case 1
        % Play animation
        play_animation(y1, y2, time, animation_name, t_steps_jump, part_size, L1, L2);
    case 2
        % Save animation
        save_animation(y1, y2, time, animation_name, t_steps_jump, part_size, L1, L2, frame_rate);
    otherwise
        disp('Invalid choice.');
end

function play_animation(y1, y2, time, ~, t_steps_jump, part_size, L1, L2)
    % Loop through time steps and update the plot dynamically
    for i = 1:size(y1, 1)
        scatter(y1(i, :), y2(i, :), part_size, 'filled');
        xlabel('y1 [m]'); xlim([0 L1]);
        ylabel('y2 [m]'); ylim([0 L2]);
        daspect([L1 L2 1]);
        title(['Positions of Particles at t = ', num2str(time(i*t_steps_jump)), 's']);
        grid on;
        axis equal;
        pause(0.01); % Adjust the pause time as needed for desired animation speed
    end
end

function save_animation(y1, y2, time, animation_name, t_steps_jump, part_size, L1, L2, frame_rate)
    % Create a video writer object
    writerObj = VideoWriter(animation_name, 'MPEG-4');
    writerObj.FrameRate = frame_rate; % Set the frame rate (adjust as needed)
    open(writerObj);

    % Loop through time steps and update the plot dynamically
    for i = 1:size(y1, 1)
        scatter(y1(i, :), y2(i, :), part_size, 'filled');
        xlabel('y1 [m]'); xlim([0 L1]);
        ylabel('y2 [m]'); ylim([0 L2]);
        daspect([L1 L2 1]);
        title(['Positions of Particles at t = ', sprintf('%.2f', time(i*t_steps_jump)), 's']);
        grid on;
        axis equal;
        
        % Write the current frame to the video
        frame = getframe(gcf);
        writeVideo(writerObj, frame);
    end

    % Close the video writer
    close(writerObj);
end
