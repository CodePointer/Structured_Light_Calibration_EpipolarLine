main_file_path = 'E:/Structured_Light_Data/20170414/2/';
ipro_file_path = 'ground_truth/';
jpro_file_path = 'ground_truth/';
ipro_file_name = 'ipro_mat';
jpro_file_name = 'jpro_mat';
ipro_file_suffix = '.txt';
jpro_file_suffix = '.txt';

%% Load data
% ipro_mats = zeros(1024, 1280, 30);
% jpro_mats = zeros(1024, 1280, 30);
% for i = 1:30
%     ipro_mats(:,:,i) = load([main_file_path, ...
%         ipro_file_path, ...
%         ipro_file_name, ...
%         num2str(i), ...
%         ipro_file_suffix]);
%     jpro_mats(:,:,i) = load([main_file_path, ...
%         jpro_file_path, ...
%         jpro_file_name, ...
%         num2str(i), ...
%         jpro_file_suffix]);
% end

%% Count Values
% point_extract = cell(1024, 1280);
% for h = 1:1024
%     for w = 1:1280
%         point_extract{h, w} = [];
%         for i = 1:30
%             if (ipro_mats(h, w, i) > 0) && (jpro_mats(h, w, i) > 0)
%                 point_extract{h, w} = [point_extract{h, w}, ...
%                     [ipro_mats(h, w, i); jpro_mats(h, w, i)]];
%             end
%         end
%     end
% end

%% Calculate A,B,C
para_A_mat = zeros(1024, 1280);
para_B_mat = zeros(1024, 1280);
para_C_mat = ones(1024, 1280);
valid_mat = zeros(1024, 1280);
for h = 1:1024
    for w = 1:1280
        point_list = point_extract{h, w};
        if (size(point_list, 2) > 0) && (max(point_list(1, :)) - min(point_list(1, :)) > 10)
            valid_mat(h, w) = 1;
            % Calculate Epipolar Line
            tmp_M = point_list';
            tmp_Y = - ones(size(point_list, 2), 1);
            tmp_X = tmp_M \ tmp_Y;
            para_A_mat(h, w) = tmp_X(1);
            para_B_mat(h, w) = tmp_X(2);
%             figure(1), plot(tmp_M(:,1), tmp_M(:,2), '.');
%             hold on, plot(tmp_M(:,1), -(tmp_X(1)/tmp_X(2))*tmp_M(:,1)-(1/tmp_X(2)));
%             hold off
        end
    end
end

save valid_mat.mat valid_mat
save para_A_mat.mat para_A_mat
save para_B_mat.mat para_B_mat