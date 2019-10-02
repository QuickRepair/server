import random


class Coordinate(object):
    def __init__(self):
        self._longitude = random.uniform(0, 360)
        self._latitude = random.uniform(0, 360)

    def longitude(self):
        return self._longitude

    def latitude(self):
        return self._latitude
