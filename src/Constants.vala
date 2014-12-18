/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/


namespace Constants {
    public static char FORMULA_START = '{';
    public static char FORMULA_END = '}';
    public static char CLAUSE_START = '{';
    public static char CLAUSE_END = '}';
    public static char CLAUSE_DELIMITER = ' ';
    public static char LITERAL_DELIMITER = ',';
    public static char NEGATE_CHAR = '~';
    
    public static bool COLOR_ENABLED = true;
    public static const string COLOR_PREFIX_FALSE = "\033[;91m";
    public static const string COLOR_PREFIX_TRUE = "\033[;92m";
    public static const string COLOR_SUFFIX = "\033[0;49;39m";
}
