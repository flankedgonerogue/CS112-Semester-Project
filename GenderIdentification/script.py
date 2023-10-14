import pickle
import sys


def gender_features(word):
    return {'last_letter': word[-1],
            'last_two_letters': word[-2:],
            'first_letter': word[1],
            'first_two_letter': word[:2]}


classifier_file = open(sys.argv[1] + '\\GenderClassifier.pickle', 'rb')
classifier = pickle.load(classifier_file)
classifier_file.close()

print("Identifying gender according to the following classifier data:")

# Most informative feature sets
classifier.show_most_informative_features(10)

# Testing
in_file = open(sys.argv[1] + '\\input.txt', 'r')
out_file = open(sys.argv[1] + '\\output.txt', 'w')
for line in in_file:
    if len(line) == 0:
        continue

    line = line.strip('\n')
    gender = classifier.classify(gender_features(line))
    out_file.write(gender + '\n')
