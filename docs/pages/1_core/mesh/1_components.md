# Components     {#mesh_components_page}

A Component is a C++ class that describes a piece of data of an Element or of a Mesh, and exposes a list of member functions that allow to manipulate it. An example of Component is the vcl::comp::Color class: it describes a vcl::Color, and exposes some member functions that allows to access that color: vcl::comp::Color::color().

The data of the Components of an Element is not stored inside the Element: it is stored by its Container (see @ref mesh_containers_page), in a contiguous array for each Component. A Component is therefore the *description* of the data (its type, the identifier of the Component, if it is optional or not) and of the member functions that the Elements that have that Component expose. Every Component has an identifier, listed in the vcl::CompId enumeration, that can be used to access to the Component in a generic way, e.g. `element.template component<vcl::CompId::POSITION>()`.

Components are combined with each other to form more complex structures, which provide all the member functions of the components they are composed of. These structures can be Elements or Meshes: VCLib Components are written in order to be as generic as possible, and in order to be used properly in all the possible contexts.

Components can be optional: in that case, their data is allocated by the Container only when they are enabled (see @ref core_mesh).

Components are listed in the @ref components group, and they are defined under the vcl::comp namespace. The names of the Components that can be used by a specific Element are also available in the namespace of the Element (e.g. `vcl::vert::Position3d`, `vcl::face::OptionalColor`, `vcl::edge::Qualityf`).
