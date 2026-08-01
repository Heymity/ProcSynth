[y, Fs] = audioread("C:\Users\GABRIEL\Downloads\single-piano-note-a4_100bpm_C_major.wav");


% Convert stereo to mono if necessary
if size(y, 2) > 1
    y = mean(y, 2); 
end

% 2. Define STFT parameters (1024 frequency bins)
windowLength = 1024;
win = hamming(windowLength);
overlap = 512; % 50% overlap

% 3. Compute the complex STFT matrix
% S contains the complex frequency components (Rows = Bins, Cols = Time frames)
[S, f, t] = stft(y, Fs, 'Window', win, 'OverlapLength', overlap, 'FFTLength', windowLength);

figure;
stft(y, Fs, 'Window', win, 'OverlapLength', overlap, 'FFTLength', windowLength);
title('Spectrogram (STFT Magnitude)');

% 1. Calculate magnitude matrix to identify dominant components
magnitudeMatrix = abs(S);

% 2. Find the bin index with the highest magnitude for each time frame
[~, maxBinIndices] = max(magnitudeMatrix, [], 1);

% 3. Extract the original complex values for these dominant bins
mainComplexFrequencies = zeros(size(maxBinIndices));
for t_idx = 1:length(maxBinIndices)
    dominant_bin = maxBinIndices(t_idx);
    mainComplexFrequencies(t_idx) = S(dominant_bin, t_idx);
end

% 4. Export the resulting complex vector to a workspace variable or CSV
% Row 1: Complex numbers, Row 2: Corresponding physical frequency in Hz
exportedData = [mainComplexFrequencies; f(maxBinIndices)'];
save('main_complex_frequencies.mat', 'exportedData');