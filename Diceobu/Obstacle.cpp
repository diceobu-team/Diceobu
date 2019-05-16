/* This is the Obstacle class source file.
 * It contains the definition of the
 * Obstacle class Constructor and other
 * Member Functions.
 */

 //	User Libraries
#include "Obstacle.h"
#include "Object.h"
//	Standard Libraries
#include <string>

//	Constructors
Obstacle::Obstacle(const std::string	&name,		const int			&hitPoints,
				   const int			&overheal,	const int			&armorClass,
				   const std::string	&size,		const int			&height,
				   const int			&weight,	const int			&coordinates,
				   const int			&entityID,	const std::string	&material)
	:	Object(name, hitPoints, overheal, armorClass, size, height, weight, coordinates, entityID),
		m_material{ material }
{}
//	Others
