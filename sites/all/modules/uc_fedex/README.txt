About
=====
This module is an implementation of FedEx shipping quotes for Ubercart.  It
allows you to quote FedEx shipping costs to your customer during checkout,
and lets your customer select FedEx services from among the shipping choices.

It gives administrators the ability to specify which FedEx services to quote,
along with various options regarding packaging and pickup/dropoff.  The module
will submit, retrieve, and display a quote in the checkout page when the user
clicks on "Calculate Shipping Rates" or when a shipping addreses is entered.

Also included is a function that lets you track packages and display the
tracking information - this is not interfaced into the store menus, it is
provided merely for reference.  A completely integrated tracking solution for
FedEx, UPS, and USPS may be found at http://drupal.org/project/uc_tracking.


This module still has limitations and some hardwired values, which I've tried
to document here and in the code.  


Quick Start
===========
Check requirements:  PHP 5 built with --enable-soap, Drupal 5.3, Ubercart
Alpha 8.

Disable any previous version of uc_fedex.  Copy this tarball into your
sites/all/modules directory and unzip/untar it.

In your web browser, navigate to admin/build/modules and enable the FedEx
module.  Go to admin/store/settings/quotes/methods/fedex and enter the required
information.  Finally, enable FedEx quotes at admin/store/settings/quotes/methods

If this doesn't work, read the rest of this document (which you really should
have done first, anyway!).


Features
========
The quote is based on store zip code for the origination address, and customer
zip code (or country and zone, in the case of non-US customers) for the
destination address.  THE STORE ADDRESS MUST BE SET!  Check that now, I'll
wait... Products are divided into packages in order to keep the total package
weight below the FedEx weight limit (150lbs).  Quotes returned reflect total
shipping cost for all the packages in an order.

An admin menu option lets you chose a "Weight Markup" to be applied to every
order - this can adjust the order total weight based on a percentage, a
multiplier, or an addition, and is meant to account for the additional weight
of your packing materials.  A rate markup is also provided, to adjust the
shipping rate based on a percentage, a multiplier, or an addition. The rate
markup is used to compensate for handling and other expenses you may incur
that you want to lump in with the shipping cost.  

The admin has the option of choosing Residential or Commercial quotes.
Shipping using the FedEx residential service costs a little more.  Shipping
to a residential address using the *commercial* service costs more plus
there's an added penalty.  So if you ship mainly to residences (whether
there is a business at that residence or not), use the Residential quotes
(default).  If your customers are mostly or all commercial addresses, you
may want to use the commercial quotes to present a slightly lower rate to
your customer.  Note that this selection does not actually affect the amount
you pay to FedEx for the package, but it DOES affect how much you collect
from your customer for shipping charges.

Tracking information may be obtained using the uc_fedex_tracking_request()
function.  An example of how to use this function is in the code comments.
A completely integrated tracking solution for FedEx, UPS, and USPS is
provided by the uc_tracking module, which may be found at
http://drupal.org/project/uc_tracking.


Requirements
============
PHP 5 built with --enable-soap is REQUIRED for this to work.

The FedEx Web Services API uses SOAP over HTTP for communication with the
FedEx Server.  SOAP is a standard extension to PHP 5, but is only poorly
supported via a contributed library for PHP 4. Because of this, and because
Drupal is already planning on requiring PHP 5.2 (in version 7.0), and because
PHP 5 is widely available and several years old already, I have decided that
it doesn't make sense to try to force things to work with PHP 4.  So if you
need to use this module, you'll have to be running PHP 5.

I've tested it with all versions of Drupal 5.3 and greater, and all versions
of Ubercart 1.0 Alpha 8 and greater.  While it may run in older versions, I
won't promise anything.  Future version of this module WILL be backwards
compatible with the initial release.


Before You Begin
================
Rate quotes won't work until you obtain FedEx developer credentials.  These
credentials are easy (and free!) to obtain - you simply have to register on
the FedEx web site and you will have them in minutes.  FedEx credentials
consist of a Key, Password, Account Number, and Meter Number, and need to be
entered in the admin form at  admin/store/settings/quotes/methods/fedex once
you have installed the module.  There are two types of credentials - Test and
Production.  You will need to get your Test credentials first, before you
will be issued Production credentials.  

Test credentials allow you to make transactions on the FedEx Test Server.
I strongly suggest you do all your development on the Test server and move to
the Production server only when ready to go live.  While this module does NOT
make any transactions that will incur charges to your FedEx account, testing
can generate a lot of bogus and perhaps invalid transactions that are best
kept off the production machine.  Remember, YOU are responsible for your
FedEx developer credentials - if this module causes problems on the production
server YOU will be the one contacted.  I think we would both prefer that the
bugs were worked out on the Test Server!

Your Test credential Account Number is *not* the same as your normal FedEx
Shipper Account Number; because of this, any discount rates associated with
your Shipper Account will not be reflected in the rate quotes returned by the
Test server.  (The Production server, however, uses your normal Account Number
and *will* provide both list rates and discount rates.)

To obtain your FedEx Test Credentials, first register at:
http://www.fedex.com/us/developer/
Then navigate to:
https://www.fedex.com/wpor/web/jsp/drclinks.jsp?links=develop.html and fill
in the form.  Your credentials should be presented to you immediately
followed by the same information split into two separate e-mails.  Also
included will be instructions for obtaining your Production credentials,
when you need them.


Installation
============
Before you use this module, disable any previous version of uc_fedex,
then remove that code from your machine.

Copy the tar.gz archive for this module into your sites/all/modules directory
and unzip/untar it.

In your web browser, navigate to admin/build/modules and enable the FedEx
module.  Go to admin/store/settings/quotes/methods/fedex and enter the required
information.  Finally, enable FedEx quotes admin/store/settings/quotes/methods

You should now be receiving FedEx quotes on your checkout page.  If you still
have problems see the "Troubleshooting" section below.


Limitations
===========
The package size is hardwired to 1"x1"x1" - this is the size FedEx uses
on their own web page to deliver a "quick quote".  A more detailed quote
requires actual package dimensions.  FedEx defines the "dimensional weight"
(in pounds) of a package as length x width x height in inches, divided by 194
for shipments within US (for international shipments divide by 166).  For
this hardwired box size, that comes out to a "dimensional weight" of less than
1 ounce.  FedEx will charge based on the greater of the actual weight and the
dimensional weight.  The quote presented to the customer ignores the
dimensional weight, and just returns the rate based on your package weight.
For large but light objects, where the dimensional weight should be used
instead, you'll have to enter the product weight a bit higher than actual -
i.e. you'll have to enter the dimensional weight on the product page rather
than the actual weight in order to receive accurate quotes.

Drop-shipping will be included, but for now everything ships from the store
address.

This module works for sending packages from US to other countries. Quote
type MUST be set to ACCOUNT for international quotes to work.

I still have to decide how to handle the shipping date for the quote - it's
not as simple as entering the current day because, say, your customer chooses
"Next Day" delivery, but it's already after the shipping cutoff time - then
the ship date is NOT "today".  You don't want to be obligated to ship under
those conditions - I'm working on an admin config for the module that lets
you specify cutoff times and closed days for your store.


Troubleshooting
===============
Does your site have PHP 5 built with --enable-soap?  Execute <?php phpinfo()
?> to see the details of your PHP installation.

You did set your store address at admin/store/settings/store/edit, didn't you?

Check to see that you have entered the correct developer credentials - Test
credentials for the Test server, Production credentials for the Production
server.

Check the box "Display debug information to administrators" in the menu at
admin/store/settings/quotes/edit .  This will print debug information on the
checkout page, including the full request sent to the FedEx server and the
complete response.  Examine these lines carefully for any hints of what is
going wrong.

Read the comments in the code - there are some debugging print statements left
in that can be uncommented if you have problems, and there is a menu option in
the shipping quotes admin pane which lets you enable some other debug info.
