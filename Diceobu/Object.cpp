/* This is the Object class source file.
 * It contains the definition of the
 * Object class Constructor and other
 * Member Functions.
 */

//	User Libraries
#include "Object.h"
//	Standard Libraries
#include <string>

//	Constructors
Object::Object(const std::string	&name, const int	&hitPoints,
	const int			&overheal, const int	&armorClass,
	const std::string	&size, const int	&height,
	const int			&weight, const int	&coordinates,
	const int			&entityID)
	:	Entity(name, hitPoints, overheal, armorClass,
		size, height, weight, coordinates, entityID)
{}
//	Others
