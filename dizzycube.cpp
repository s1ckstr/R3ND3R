#include "R3ND3R.h"

using namespace R3ND3R;
using namespace std;

int main() {
    Engine r3nd3r;
    float theta = 0.0f;

    vector<pair<Vec3, Vec3>> edges = {
        {{-1, -1, -1}, {1, -1, -1}}, {{1, -1, -1}, {1, 1, -1}}, 
        {{1, 1, -1}, {-1, 1, -1}}, {{-1, 1, -1}, {-1, -1, -1}},
        {{-1, -1, 1}, {1, -1, 1}}, {{1, -1, 1}, {1, 1, 1}}, 
        {{1, 1, 1}, {-1, 1, 1}}, {{-1, 1, 1}, {-1, -1, 1}},
        {{-1, -1, -1}, {-1, -1, 1}}, {{1, -1, -1}, {1, -1, 1}}, 
        {{1, 1, -1}, {1, 1, 1}}, {{-1, 1, -1}, {-1, 1, 1}}
    };

    while (true) {
        r3nd3r.Clear();

        for (auto& edge : edges) {
            Vec3 p1 = edge.first;
            Vec3 p2 = edge.second;

            // standard rotation matrix math
            auto rotate = [&](Vec3& p) {
                float x = p.x * cos(theta) - p.z * sin(theta);
                float z = p.x * sin(theta) + p.z * cos(theta);
                p.x = x; p.z = z;

                float y = p.y * cos(theta * 0.5f) - p.z * sin(theta * 0.5f);
                z = p.y * sin(theta * 0.5f) + p.z * cos(theta * 0.5f);
                p.y = y; p.z = z;
            };

            rotate(p1);
            rotate(p2);

            //depth fading happens here (wow effect)
            p1.z += 2.5f; 
            p2.z += 2.5f;

            r3nd3r.DrawLine3D(p1, p2);
        }

        r3nd3r.Display();
        
        theta += 0.05f;
        Sleep(16); 
    }

    return 0;
}
