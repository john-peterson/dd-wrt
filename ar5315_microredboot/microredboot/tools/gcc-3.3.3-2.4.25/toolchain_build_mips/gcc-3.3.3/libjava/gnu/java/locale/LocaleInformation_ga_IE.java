// This file was automatically generated by localedef.

package gnu.java.locale;

import java.util.ListResourceBundle;

public class LocaleInformation_ga_IE extends ListResourceBundle
{
  static final String decimalSeparator = ".";
  static final String groupingSeparator = ",";
  static final String numberFormat = "#,###,##0.###";
  static final String percentFormat = "#,###,##0%";
  static final String[] weekdays = { null, "D\u00E9 Domhnaigh", "D\u00E9 Luain", "D\u00E9 M\u00E1irt", "D\u00E9 C\u00E9adaoin", "D\u00E9ardaoin", "D\u00E9 hAoine", "D\u00E9 Sathairn" };

  static final String[] shortWeekdays = { null, "Domh", "Luan", "M\u00E1irt", "C\u00E9ad", "D\u00E9ar", "Aoine", "Sath" };

  static final String[] shortMonths = { "Ean", "Feabh", "M\u00E1rta", "Aib", "Beal", "Meith", "I\u00FAil", "L\u00FAn", "MF\u00F3mh", "DF\u00F3mh", "Samh", "Noll", null };

  static final String[] months = { "Ean\u00E1ir", "Feabhra", "M\u00E1rta", "Aibre\u00E1n", "M\u00ED na Bealtaine", "Meith", "I\u00FAil", "L\u00FAnasa", "Me\u00E1n F\u00F3mhair", "Deireadh F\u00F3mhair", "M\u00ED na Samhna", "M\u00ED na Nollag", null };

  static final String[] ampms = { "", "" };

  static final String shortDateFormat = "dd.MM.yy";
  static final String defaultTimeFormat = "";
  static final String currencySymbol = "\u00A3";
  static final String intlCurrencySymbol = "IEP";
  static final String currencyFormat = "$#,###,##0.00;-$#,###,##0.00";

  private static final Object[][] contents =
  {
    { "weekdays", weekdays },
    { "shortWeekdays", shortWeekdays },
    { "shortMonths", shortMonths },
    { "months", months },
    { "ampms", ampms },
    { "shortDateFormat", shortDateFormat },
    { "defaultTimeFormat", defaultTimeFormat },
    { "currencySymbol", currencySymbol },
    { "intlCurrencySymbol", intlCurrencySymbol },
    { "currencyFormat", currencyFormat },
    { "decimalSeparator", decimalSeparator },
    { "groupingSeparator", groupingSeparator },
    { "numberFormat", numberFormat },
    { "percentFormat", percentFormat },
  };

  public Object[][] getContents () { return contents; }
}
