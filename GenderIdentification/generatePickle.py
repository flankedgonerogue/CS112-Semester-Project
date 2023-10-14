import os
import pickle
import random

import nltk.data
from nltk import apply_features, NaiveBayesClassifier
from nltk.corpus import names

print(os.getcwd())

nltk.data.path.append(os.getcwd() + "\\nltk_data")


def gender_features(word):
    return {'last_letter': word[-1],
            'last_two_letters': word[-2:],
            'first_letter': word[1],
            'first_two_letter': word[:2]}


# Read the names from the files.
# Label each name with the corresponding gender.
male_names = [(name, 'male') for name in names.words('male.txt')]
female_names = [(name, 'female') for name in names.words('female.txt')]

# Combine the lists.
labeled_names = male_names + female_names

# Shuffle the list.
random.shuffle(labeled_names)

# Apply features to the training set
train_set = apply_features(gender_features, labeled_names)

# Train a Naive Bayes classifier
classifier = NaiveBayesClassifier.train(train_set)

classifier_file = open('GenderClassifier.pickle', 'wb')
pickle.dump(classifier, classifier_file)
classifier_file.close()

# Most informative feature sets
classifier.show_most_informative_features(50)
