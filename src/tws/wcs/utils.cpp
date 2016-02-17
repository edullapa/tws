#include "utils.hpp"

rapidxml::xml_node<char>* tws::wcs::initialize_wcs_xml_namespaces(rapidxml::xml_document<char>& doc)
{
  rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "wcs:CoverageDescriptions");

  rapidxml::xml_attribute<>* attr = doc.allocate_attribute("xmlns:wcs", "http://www.opengis.net/wcs/2.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:ows", "http://www.opengis.net/ows/2.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:gml", "http://www.opengis.net/gml/3.2");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:gmlcov", "http://www.opengis.net/gmlcov/1.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xmlns:swe", "http://www.opengis.net/swe/2.0");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("version", "2.0.1");
  root->append_attribute(attr);

  attr = doc.allocate_attribute("xsi:schemaLocation", "http://www.opengis.net/wcs/2.0 http://schemas.opengis.net/wcs/2.0/wcsGetCapabilities.xsd");
  root->append_attribute(attr);

  return root;
}
