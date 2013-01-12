
-- SUMMARY --

The Synonyms module extends the Drupal core Taxonomy features. Currently
the module provides this additional functionality:
* support of synonyms. Any field attached to term can be enabled as source of
   synonyms. Please, see below for the list of field types currently supported
   for extraction.
* synonym-friendly autocomplete widget for taxonomy_term_reference fields
* integration with Drupal search functionality enabling searching content by
   synonyms of the terms that the content references

-- REQUIREMENTS --

The Synonyms module requires enabled:
* Taxonomy module
* Text module

-- SYNONYMS SOURCE FIELDS (API) --

Module ships with ability to extract synonyms from the following field types:
* Text
* Taxonomy Term Reference
* Entity Reference
* Number
* Float
* Decimal

If you want to implement your own synonyms extractor that would enable support
for any other field type, please, refer to synonyms.api.php file for
instructions on how to do it, or file an issue against Synonyms module. We will
try to implement support for your field type too. If you have written your
synonyms extractor, please share by opening an issue, and it will be included
into this module.

-- INSTALLATION --

* Install as usual

-- CONFIGURATION --

* The module itself does not provide any configuration as of the moment.
Although during creation/editing of a Taxonomy vocabulary you will be able
to enable/disable for that particular vocabulary the additional functionality
this module provides.
