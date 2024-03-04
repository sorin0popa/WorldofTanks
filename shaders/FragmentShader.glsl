#version 330

// Output
layout(location = 0) out vec4 out_color;

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
flat in int nr_lives1;

vec3 deformare_culoare(int nr_lives, vec3 color) {

        float factor_deformare = (3 - nr_lives) * 0.2;

        vec3 new_color;
            
        new_color.x = color.x - factor_deformare;
        new_color.z = color.z - factor_deformare;
        new_color.y = color.y - factor_deformare;
        return new_color;
    }

void main()
{
    // TODO(student): Write pixel out color
    if (nr_lives1 != -1 && nr_lives1 != 3) {
    vec3 new_color = deformare_culoare(nr_lives1, color);
    out_color = vec4(new_color, 1.f);
    }
    else {
        out_color = vec4(color, 1.f);
    }

}
