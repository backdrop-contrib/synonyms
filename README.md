Synonyms
========

**Please note:
This port from the Drupal 7 module is not complete nor fully tested. Your 
help and input would be appreciated!**

The Synonyms module enriches Entities with the notion of synonyms. Currently the
module provides the following functionality:
- support of synonyms through Field API. Any field, for which synonyms behavior
  implementation exists, can be enabled as source of synonyms.
- support of synonyms through Entity properties. Entity properties stored in
  database (as opposed to being calculated on-the-fly) can be enabled as source
  of synonyms.
- synonyms-friendly autocomplete and select widgets for taxonomy_term_reference,
  entityreference, and commerce_product_reference (through Synonyms Commerce
  submodule) fields.
- integration with core search functionality through Synonyms Search
  submodule. It enables searching content by synonyms of the terms that the
  content references. Synonyms Search submodule also integrates with Term Search
  contributed module in a fashion that allows your terms to be found by their
  synonyms.
- integration with Search API. If you include entity synonyms into your Search
  API search index, your clients will be able to find content with search
  keywords that contain synonyms and not actual names of entities.
- integration with Views. Synonyms module provides a few filters and contextual
  filters that allow filtering not only by entity name but also by one of its
  synonyms. Synonyms module also provides a Views field for all eligible
  entities that contains a list of synonyms associated with the entity in
  question.

Requirements
------------

The Synonyms module requires the following modules:
- Entity 
- Entity Plus
- Plugin Manager

The Synonyms module integrates with (but does not require) the following
modules:
- Taxonomy
- Search
- Term Search
- Entity Reference
- Views
- Commerce
- Features
- Term Merge

Supported synonyms providers
----------------------------

Module ships with ability to provide synonyms from the following locations:
- "Text" field type
- "Taxonomy Term Reference" field type
- "Entity Reference" field type
- "Commerce Product Reference" field type
- "Commerce price" field type
- "Number" field type
- "Float" field type
- "Decimal" field type
- Entity properties stored in database

Worth mentioning here: this list is easily extended further by implementing new
synonyms providers in your code. Refer to Synonyms advanced help for more
details on how to accomplish it.

Granulation within synonyms behavior
------------------------------------

In order to achieve greater flexibility, this module introduced additional
granularity into what "synonyms" mean. This granularity is expressed via
"synonyms behavior" idea whatsoever. Then you can enable different synonyms
behaviors for different synonyms providers. For example, field "Typos" can be
part of autocomplete behavior, while field "Other spellings" can be part of
search integration behavior. Currently the following synonym behaviors are
recognized (other modules actually can extend this list):
- Autocomplete - whether synonyms from this provider should participate in
  autocomplete suggestions. This module ships with autocomplete synonyms
  friendly widgets and their autocomplete suggestions will be filled in with the
  synonyms of providers that have this behavior enabled.
- Select - whether synonyms from this provider should be included in the
  synonyms friendly select widgets.
- Search integration (requires Synonyms Search enabled) - allows your content to
  be found by synonyms of the terms it references. Your nodes will be found by
  all synonyms that have this behavior enabled.

Therefore, on the Synonyms configuration page you will see a table, where rows
are synonym providers and columns are these "synonym behaviors" and you decide
what synonym behaviors to activate on what synonym providers.

Installation
------------
Install this module using the official Backdrop CMS instructions at https://backdropcms.org/guide/modules.

Configuration
-------------
You can configure synonyms of all eligible entity types by going to Admin ->
Structure -> Synonyms (admin/structure/synonyms)

License
-------
This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

Maintainers
-----------
- Alex Höbart (https://github.com/AlexHoebart-ICPDR)

Originally written for Drupal by
- Bojan Živanović (bojanz) (https://www.drupal.org/u/bojanz)
- Derek Wright
- Alexander Trotsenko

This module is seeking additional maintainers.
