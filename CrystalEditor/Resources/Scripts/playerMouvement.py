from CrystalPython import *

class RotateObject(ObjectScript):
    def Start(self):
        self.sensibility = 1

    def Update(self):
        self.transform.RotateEuler(Vector3(0, self.time.DeltaTime() * PI/8 , 0))