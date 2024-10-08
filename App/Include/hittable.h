#ifndef HITTABLE_H
#define HITTABLE_H

class hit_record {
public:
	glm::vec3 point;
	glm::vec3 normal;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const glm::vec3& outward_normal) {
		// Sets the hit record normal vector.
		// NOTE: outward_normal is assumed to have unit length
		
		front_face = glm::dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif
