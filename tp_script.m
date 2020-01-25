path_trace_folder = './Supports analyse statistique/Traces/'; % path for the folder containing all the traces
path_plain_text = './Supports analyse statistique/pText.txt'; % path for the plain text used
path_key = './Supports analyse statistique/key_150_5000.txt'; % path of the file in which the key will be written
path_subbytes_matrix = './Supports analyse statistique/AES_SubBytes_Table.mat'; % path of the S-Box (.mat format)
nb_trace = 150; % number of traces saved
nb_points = 5000; % number of points for each trace
nb_bytes = 16; % lenght of the key in bytes

matrix_trace = getTraces(path_trace_folder, nb_trace, nb_points); % matrix of all the traces (one line = one trace)
matrix_plain_text_dec = getPlainText(path_plain_text, nb_trace, nb_bytes); % matrix of all the plain texts (one line = one plain text)
matrix_subbytes = load(path_subbytes_matrix, '-mat'); % structure where "SubBytes" is the S-Box
[key, vec_corr, vec_time] = getHypothesis(matrix_trace, matrix_plain_text_dec, matrix_subbytes.SubBytes, nb_trace, nb_bytes, nb_points);
writeResults(key, vec_corr, path_key);
plotLeakTime(nb_points, matrix_trace, vec_time);

function plotLeakTime(p_nb_points, p_matrix_traces, p_vec_time)
    % Plot the time of each data leak
    x = linspace(0, p_nb_points, p_nb_points);
    y1 = p_matrix_traces(1, :);
    plot(x, y1);
    hold on
    y2 = p_vec_time;
    plot(x, y2);
    y3 = -p_vec_time;
    plot(x, y3);
    hold off
end

function mat = getTraces(path, p_nb_trace, p_nb_points)
    % Load all the matrix traces in a global matrix (and switch lines to columns)
    mat = zeros(p_nb_trace, p_nb_points);
    for i=1:p_nb_trace
        path_current_trace = append(path, 'Traces_cm (', int2str(i), ').mat');
        T = load(path_current_trace, '-mat');
        for j=1:p_nb_points
            mat(i,j) = T.A(j, 1);
        end
    end
end

function mat = getPlainText(p_ptext_path, p_nb_trace, p_nb_bytes)
    % Load the plain text (hex format) in a matrix (dec format)
    mat = zeros(p_nb_trace, p_nb_bytes);
    fid = fopen(p_ptext_path);
    for i = 1:p_nb_trace
        tline = fgetl(fid); % current line
        tmp_line = cellstr(reshape(tline,2,[])'); % split current line each 2 characters, e.g. "0A22FF1C" will become ['0A' '22' 'FF' '1C']
        for j = 1:p_nb_bytes
            mat(i, j) = hex2dec(tmp_line(j));
        end
    end
end

function [hyp_key, vec_corr, vec_time] = getHypothesis(p_matrix_traces, p_matrix_plain_text, p_sbox, p_nb_trace, p_nb_bytes, p_nb_points)
    % Retrieve the key according to all the parameters: Matrix of all
    % traces (and their parameters), matrix of plain text used, S-Box used
    mat_hamming = zeros(256, p_nb_trace); % Hamming weight matrix
    mat_corr = zeros(256, p_nb_points); % Correlation coefficients matrix
    hyp_key = ""; % key
    vec_corr = ""; % vector containing all the correlation coefficients
    vec_time = zeros(p_nb_points, 1);
    for i = 1:p_nb_bytes % 1 to size of key in bytes
        for j = 0:255 % 0 to 255 i.e. key byte hypothesis
            for k = 1:p_nb_trace % 1 to nb_trace i.e. number of traces i.e. line in pText.txt
                % https://fr.mathworks.com/help/matlab/ref/bitxor.html
                xor = bitxor(j, p_matrix_plain_text(k,i));
                out_sbox = p_sbox(xor + 1); % +1 because matlab starts array at 1...
                hamming_weight = sum(dec2bin(out_sbox).' == '1'); % Hamming weight
                mat_hamming(j+1, k) = hamming_weight;
            end
        end % at this point we have the hamming weight matrice for one column in the plain text file (i.e. the line are for key hypethosis, and the column for a specific trace)
        for m = 1:p_nb_points
            for n = 0:255
                % calculus of the correlation factor
                % https://fr.mathworks.com/matlabcentral/answers/348781-how-can-i-run-correlations-from-one-row-across-several-rows-in-another-matrix
                [r,p] = corrcoef(mat_hamming(n+1, :), p_matrix_traces(:, m));
                corr_factor = r(2, 1);
                mat_corr(n+1,m) = corr_factor;
            end
        end
        maximum = max(max(mat_corr)); % find the maximum value in the matrix
        vec_corr = append(vec_corr, " ", num2str(maximum)); % Add the correlation factor for this byte
        disp(maximum);
        [x, y] = find(mat_corr == maximum); % get the coordinates (line, column) of the maximum value
        hyp_key = append(hyp_key, " ", dec2hex(x-1, 2)); % % -1 because matlab starts array at 1...
        vec_time(y) = 0.1;
        disp(dec2hex(x-1, 2));
    end
end

function writeResults(key_string, vec_corr, file_path)
    % write the given key string and its correlation string into the given file
    k_array = strsplit(key_string);
    c_array = strsplit(vec_corr);
    len = size(k_array);
    fid = fopen(file_path, 'w');
    fprintf(fid, "Key: %s", key_string);
    fprintf(fid, "\n\n\nKEY  CORR_COEF\n");
    for i = 1:len(2)
        fprintf(fid, '%s \t %s\n', k_array(i), c_array(i));
    end
    fclose(fid);
end

% clear all;
