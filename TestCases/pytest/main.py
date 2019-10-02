import unittest

test_suit = unittest.TestSuite()
cases = unittest.defaultTestLoader.discover('./', 'test_*.py')
for case in cases:
    test_suit.addTest(case)
runner = unittest.TextTestRunner()
runner.run(test_suit)
