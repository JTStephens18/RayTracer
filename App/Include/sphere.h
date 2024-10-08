#ifndef SPHERE_H
#define SPHERE_H

class sphere : public hittable {
	public:
		sphere(const glm::vec3& center, float radius) : center(center), radius(std::fmax(0, radius)) {}

		bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
			glm::vec3 oc = center - r.origin();
			auto a = length_squared(r.direction());
			auto h = glm::dot(r.direction(), oc);
			auto c = length_squared(oc) - radius * radius;
			auto discriminant = h * h - a * c;
			if (discriminant < 0) {
				return false;
			}
			auto sqrtd = std::sqrt(discriminant);

			// Find nearest root that lies in the acceptable range.
			auto root = (h - sqrtd) / a;
			if (root <= ray_tmin || ray_tmax <= root) {
				root = (h + sqrtd) / a;
				if (root <= ray_tmin || ray_tmax <= root) {
					return false;
				}
			}
			rec.t = root;
			rec.point = r.at(rec.t);
			glm::vec3 outward_normal = (rec.point - center) / radius;
			rec.set_face_normal(r, outward_normal);

			return true;
		}
	private:
		glm::vec3 center;
		float radius;
};

#endif
