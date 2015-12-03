/*
	The MIT License(MIT)

	Copyright(c) 2015 Vixen Team, Matt Guerrette

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef VIX_COLOR_H
#define VIX_COLOR_H

namespace Vixen {

	class Color
	{
	public:
		float r;
		float g;
		float b;
		float a;

		Color()
			: r(0.0f), g(0.0f), b(0.0f), a(1.0f)
		{

		}

		Color(float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a)
		{

		}


		bool operator== (const Color& other) 
		{
			return (r == other.r) &&
				   (g == other.g) &&
				   (b == other.b) &&
				   (a == other.a);
		}

		static const Color FromUString(UString);
	};

	namespace Colors {

		static const Color IndianRed = Color(0.804f, 0.361f, 0.361f, 1.0f); //RGBA: {0.804f, 0.361f, 0.361f, 1.0f}
		static const Color LightCoral = Color(0.941f, 0.502f, 0.502f, 1.0f);//RGBA: {0.941f, 0.502f, 0.502f, 1.0f}
		static const Color Salmon = Color(0.980f, 0.502f, 0.447f, 1.0f); //RGBA: {0.980f, 0.502f, 0.447f, 1.0f}
		static const Color DarkSalmon = Color(0.914f, 0.588f, 0.478f, 1.0f); //RGBA: {0.914f, 0.588f, 0.478f, 1.0f}
		static const Color LightSalmon = Color(1.0f, 0.627f, 0.478f, 1.0f); //RGBA: {1.0f, 0.627f, 0.478f, 1.0f}
		static const Color Crimson = Color(0.863f, 0.078f, 0.235f, 1.0f); //RGBA: {0.863f, 0.078f, 0.235f, 1.0f}
		static const Color Red = Color(1.0f, 0.0f, 0.0f, 1.0f); //RGBA: {1.0f, 0.0f, 0.0f, 1.0f}
		static const Color FireBrick = Color(0.698f, 0.133f, 0.133f, 1.0f); //RGBA: {0.698f, 0.133f, 0.133f, 1.0f}
		static const Color DarkRed = Color(0.545f, 0.0f, 0.0f, 1.0f); //RGBA: {0.545f, 0.0f, 0.0f, 1.0f}
		static const Color Pink = Color(1.0f, 0.753f, 0.796f, 1.0f); //RGBA: {1.0f, 0.753f, 0.796f, 1.0f}
		static const Color LightPink = Color(1.0f, 0.714f, 0.747f, 1.0f); //RGBA: {1.0f, 0.714f, 0.747f, 1.0f}
		static const Color HotPink = Color(1.0f, 0.410f, 0.747f, 1.0f); //RGBA: {1.0f, 0.410f, 0.747f, 1.0f}
		static const Color DeepPink = Color(1.0f, 0.078f, 0.576f, 1.0f);//RGBA: {1.0f, 0.078f, 0.576f, 1.0f}
		static const Color MediumVioletRed = Color(0.780f, 0.082f, 0.522f, 1.0f); //RGBA: {0.780f, 0.082f, 0.522f, 1.0f}
		static const Color PaleVioletRed = Color(0.859f, 0.439f, 0.576f, 1.0f); //RGBA: {0.859f, 0.439f, 0.576f, 1.0f}
		static const Color Coral = Color(1.0f, 0.627f, 0.478f, 1.0f); //RGBA: {1.0f, 0.627f, 0.478f, 1.0f}
		static const Color Tomato = Color(1.0f, 0.388f, 0.278f, 1.0f); //RGBA: {1.0f, 0.388f, 0.278f, 1.0f}
		static const Color OrangeRed = Color(1.0f, 0.271f, 0.0f, 1.0f); //RGBA: {1.0f, 0.271f, 0.0f, 1.0f}
		static const Color DarkOrange = Color(1.0f, 0.549f, 0.0f, 1.0f); //RGBA: {1.0f, 0.549f, 0.0f, 1.0f}
		static const Color Orange = Color(1.0f, 0.647f, 0.0f, 1.0f); //RGBA: {1.0f, 0.647f, 0.0f, 1.0f}
		static const Color Gold = Color(1.0f, 0.843f, 0.0f, 1.0f); //RGBA: {1.0f, 0.843f, 0.0f, 1.0f}
		static const Color Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f); //RGBA: {1.0f, 1.0f, 0.0f, 1.0f}
		static const Color LightYellow = Color(1.0f, 1.0f, 0.878f, 1.0f); //RGBA: {1.0f, 1.0f, 0.878f, 1.0f}
		static const Color LemonChion = Color(0.980f, 0.980f, 0.804f, 1.0f); //RGBA: {0.980f, 0.980f, 0.804f, 1.0f}
		static const Color LightGoldenRodYellow = Color(0.980f, 0.980f, 0.824f, 1.0f); //RGBA: {0.980f, 0.980f, 0.824f, 1.0f}
		static const Color PapayaWhip = Color(1.0f, 0.937f, 0.835f, 1.0f); //RGBA: {1.0f, 0.937f, 0.835f, 1.0f}
		static const Color Moccasin = Color(1.0f, 0.894f, 0.710f, 1.0f); //RGBA: {1.0f, 0.894f, 0.710f, 1.0f}
		static const Color PeachPu = Color(1.0f, 0.855f, 0.725f, 1.0f); //RGBA: {1.0f, 0.855f, 0.725f, 1.0f}
		static const Color PaleGoldenRod = Color(0.933f, 0.910f, 0.667f, 1.0f); //RGBA: {0.933f, 0.910f, 0.667f, 1.0f}
		static const Color Khaki = Color(0.941f, 0.902f, 0.549f, 1.0f); //RGBA: {0.941f, 0.902f, 0.549f, 1.0f}
		static const Color DarkKhaki = Color(0.741f, 0.718f, 0.420f, 1.0f); //RGBA: {0.741f, 0.718f, 0.420f, 1.0f}
		static const Color Lavender = Color(0.902f, 0.902f, 0.980f, 1.0f); //RGBA: {0.902f, 0.902f, 0.980f, 1.0f}
		static const Color Thistle = Color(0.847f, 0.749f, 0.847f, 1.0f); //RGBA: {0.847f, 0.749f, 0.847f, 1.0f}
		static const Color Plum = Color(0.867f, 0.627f, 0.867f, 1.0f); //RGBA: {0.867f, 0.627f, 0.867f, 1.0f}
		static const Color Violet = Color(0.933f, 0.510f, 0.933f, 1.0f); //RGBA: {0.933f, 0.510f, 0.933f, 1.0f}
		static const Color Orchid = Color(0.655f, 0.439f, 0.839f, 1.0f); //RGBA: {0.655f, 0.439f, 0.839f, 1.0f}
		static const Color Magenta = Color(1.0f, 0.0f, 1.0f, 1.0f); //RGBA: {1.0f, 0.0f, 1.0f, 1.0f}
		static const Color MediumOrchid = Color(0.729f, 0.333f, 0.827f, 1.0f); //RGBA: {0.729f, 0.333f, 0.827f, 1.0f}
		static const Color MediumPurple = Color(0.576f, 0.439f, 0.859f, 1.0f); //RGBA: {0.576f, 0.439f, 0.859f, 1.0f}
		static const Color BlueViolet = Color(0.541f, 0.169f, 0.886f, 1.0f); //RGBA: {0.541f, 0.169f, 0.886f, 1.0f}
		static const Color DarkViolet = Color(0.580f, 0.0f, 0.827f, 1.0f); //RGBA: {0.580f, 0.0f, 0.827f, 1.0f}
		static const Color DarkOrchid = Color(0.600f, 0.196f, 0.800f, 1.0f); //RGBA: {0.600f, 0.196f, 0.800f, 1.0f}
		static const Color DarkMagenta = Color(0.545f, 0.0f, 0.545f, 1.0f); //RGBA: {0.545f, 0.0f, 0.545f, 1.0f}
		static const Color Purple = Color(0.502f, 0.0f, 0.502f, 1.0f); //RGBA: {0.502f, 0.0f, 0.502f, 1.0f}
		static const Color Indigo = Color(0.294f, 0.0f, 0.510f, 1.0f); //RGBA: {0.294f, 0.0f, 0.510f, 1.0f}
		static const Color SlateBlue = Color(0.416f, 0.353f, 0.804f, 1.0f); //RGBA: {0.416f, 0.353f, 0.804f, 1.0f}
		static const Color DarkSlateBlue = Color(0.282f, 0.239f, 0.545f, 1.0f); //RGBA: {0.282f, 0.239f, 0.545f, 1.0f}
		static const Color GreenYellow = Color(0.678f, 1.0f, 0.184f, 1.0f); //RGBA: {0.678f, 1.0f, 0.184f, 1.0f}
		static const Color Chartreuse = Color(0.498f, 1.0f, 0.0f, 1.0f); //RGBA: {0.498f, 1.0f, 0.0f, 1.0f}
		static const Color LawnGreen = Color(0.486f, 0.988f, 0.0f, 1.0f); //RGBA: {0.486f, 0.988f, 0.0f, 1.0f}
		static const Color Lime = Color(0.0f, 1.0f, 0.0f, 1.0f); //RGBA: {0.0f, 1.0f, 0.0f, 1.0f}
		static const Color LimeGreen = Color(0.196f, 0.804f, 0.196f, 1.0f); //RGBA: {0.196f, 0.804f, 0.196f, 1.0f}
		static const Color PaleGreen = Color(0.596f, 0.984f, 0.596f, 1.0f); //RGBA: {0.596f, 0.984f, 0.596f, 1.0f}
		static const Color LightGreen = Color(0.565f, 0.933f, 0.565f, 1.0f); //RGBA: {0.565f, 0.933f, 0.565f, 1.0f}
		static const Color MediumSpringGreen = Color(0.0f, 0.980f, 0.604f, 1.0f); //RGBA: {0.0f, 0.980f, 0.604f, 1.0f}
		static const Color SpringGreen = Color(0.0f, 1.0f, 0.498f, 1.0f); //RGBA: {0.0f, 1.0f, 0.498f, 1.0f}
		static const Color MediumSeaGreen = Color(0.235f, 0.702f, 0.443f, 1.0f); //RGBA: {0.235f, 0.702f, 0.443f, 1.0f}
		static const Color SeaGreen = Color(0.180f, 0.545f, 0.341f, 1.0f); //RGBA: {0.180f, 0.545f, 0.341f, 1.0f}
		static const Color ForestGreen = Color(0.133f, 0.545f, 0.133f, 1.0f); //RGBA: {0.133f, 0.545f, 0.133f, 1.0f}
		static const Color Green = Color(0.0f, 0.502f, 0.0f, 1.0f); //RGBA: {0.0f, 0.502f, 0.0f, 1.0f}
		static const Color DarkGreen = Color(0.0f, 0.392f, 0.0f, 1.0f); //RGBA: {0.0f, 0.392f, 0.0f, 1.0f}
		static const Color YellowGreen = Color(0.604f, 0.804f, 0.196f, 1.0f); //RGBA: {0.604f, 0.804f, 0.196f, 1.0f}
		static const Color OliveDrab = Color(0.420f, 0.557f, 0.137f, 1.0f); //RGBA: {0.420f, 0.557f, 0.137f, 1.0f}
		static const Color Olive = Color(0.502f, 0.502f, 0.0f, 1.0f); //RGBA: {0.502f, 0.502f, 0.0f, 1.0f}
		static const Color DarkOliveGreen = Color(0.333f, 0.420f, 0.184f, 1.0f); //RGBA: {0.333f, 0.420f, 0.184f, 1.0f}
		static const Color MediumAquamarine = Color(0.400f, 0.804f, 0.667f, 1.0f); //RGBA: {0.400f, 0.804f, 0.667f, 1.0f}
		static const Color DarkSeaGreen = Color(0.561f, 0.737f, 0.561f, 1.0f); //RGBA: {0.561f, 0.737f, 0.561f, 1.0f}
		static const Color LightSeaGreen = Color(0.125f, 0.698f, 0.667f, 1.0f); //RGBA: {0.125f, 0.698f, 0.667f, 1.0f}
		static const Color DarkCyan = Color(0.0f, 0.545f, 0.545f, 1.0f); //RGBA: {0.0f, 0.545f, 0.545f, 1.0f}
		static const Color Teal = Color(0.0f, 0.502f, 0.502f, 1.0f); //RGBA: {0.0f, 0.502f, 0.502f, 1.0f}
		static const Color Aqua = Color(0.0f, 1.0f, 1.0f, 1.0f); //RGBA: {0.0f, 1.0f, 1.0f, 1.0f}
		static const Color Cyan = Color(0.0f, 1.0f, 1.0f, 1.0f); //RGBA: {0.0f, 1.0f, 1.0f, 1.0f}
		static const Color LightCyan = Color(0.878f, 1.0f, 1.0f, 1.0f); //RGBA: {0.878f, 1.0f, 1.0f, 1.0f}
		static const Color PaleTurquoise = Color(0.686f, 0.933f, 0.933f, 1.0f); //RGBA: {0.686f, 0.933f, 0.933f, 1.0f}
		static const Color Aquamarine = Color(0.498f, 1.0f, 0.831f, 1.0f); //RGBA: {0.498f, 1.0f, 0.831f, 1.0f}
		static const Color Turquoise = Color(0.251f, 0.878f, 0.816f, 1.0f); //RGBA: {0.251f, 0.878f, 0.816f, 1.0f}
		static const Color MediumTurquoise = Color(0.282f, 0.820f, 0.800f, 1.0f); //RGBA: {0.282f, 0.820f, 0.800f, 1.0f}
		static const Color DarkTurquoise = Color(0.0f, 0.808f, 0.820f, 1.0f); //RGBA: {0.0f, 0.808f, 0.820f, 1.0f}
		static const Color CadetBlue = Color(0.373f, 0.620f, 0.627f, 1.0f); //RGBA: {0.373f, 0.620f, 0.627f, 1.0f}
		static const Color SteelBlue = Color(0.275f, 0.510f, 0.706f, 1.0f); //RGBA: {0.275f, 0.510f, 0.706f, 1.0f}
		static const Color LightSteelBlue = Color(0.690f, 0.769f, 0.871f, 1.0f); //RGBA: {0.690f, 0.769f, 0.871f, 1.0f}
		static const Color LightBlue = Color(0.678f, 0.847f, 0.902f, 1.0f); //RGBA: {0.678f, 0.847f, 0.902f, 1.0f}
		static const Color PowderBlue = Color(0.690f, 0.878f, 0.902f, 1.0f); //RGBA: {0.690f, 0.878f, 0.902f, 1.0f}
		static const Color SkyBlue = Color(0.529f, 0.808f, 0.922f, 1.0f); //RGBA: {0.529f, 0.808f, 0.922f, 1.0f}
		static const Color LightSkyBlue = Color(0.529f, 0.808f, 0.980f, 1.0f); //RGBA: {0.529f, 0.808f, 0.980f, 1.0f}
		static const Color DeepSkyBlue = Color(0.0f, 0.749f, 1.0f, 1.0f); //RGBA: {0.0f, 0.749f, 1.0f, 1.0f}
		static const Color DodgerBlue = Color(0.118f, 0.565f, 1.0f, 1.0f); //RGBA: {0.118f, 0.565f, 1.0f, 1.0f}
		static const Color CornflowerBlue = Color(0.392f, 0.584f, 0.929f, 1.0f); //RGBA: {0.392f, 0.584f, 0.929f, 1.0f}
		static const Color MediumSlateBlue = Color(0.482f, 0.408f, 0.933f, 1.0f); //RGBA: {0.482f, 0.408f, 0.933f, 1.0f}
		static const Color RoyalBlue = Color(0.255f, 0.412f, 0.882f, 1.0f); //RGBA: {0.255f, 0.412f, 0.882f, 1.0f}
		static const Color Blue = Color(0.0f, 0.0f, 1.0f, 1.0f); //RGBA: {0.0f, 0.0f, 1.0f, 1.0f}
		static const Color MediumBlue = Color(0.0f, 0.0f, 0.804f, 1.0f); //RGBA: {0.0f, 0.0f, 0.804f, 1.0f}
		static const Color DarkBlue = Color(0.0f, 0.0f, 0.545f, 1.0f); //RGBA: {0.0f, 0.0f, 0.545f, 1.0f}
		static const Color Navy = Color(0.0f, 0.0f, 0.502f, 1.0f); //RGBA: {0.0f, 0.0f, 0.502f, 1.0f}
		static const Color MidnightBlue = Color(0.098f, 0.098f, 0.439f, 1.0f); //RGBA: {0.098f, 0.098f, 0.439f, 1.0f}
		static const Color Cornsilk = Color(1.0f, 0.973f, 0.863f, 1.0f); //RGBA: {1.0f, 0.973f, 0.863f, 1.0f}
		static const Color BlanchedAlmond = Color(1.0f, 0.922f, 0.804f, 1.0f); //RGBA: {1.0f, 0.922f, 0.804f, 1.0f}
		static const Color Bisque = Color(1.0f, 0.894f, 0.769f, 1.0f); //RGBA: {1.0f, 0.894f, 0.769f, 1.0f}
		static const Color NavajoWhite = Color(1.0f, 0.871f, 0.678f, 1.0f); //RGBA: {1.0f, 0.871f, 0.678f, 1.0f}
		static const Color Wheat = Color(0.961f, 0.871f, 0.702f, 1.0f); //RGBA: {0.961f, 0.871f, 0.702f, 1.0f}
		static const Color BurlyWood = Color(0.871f, 0.722f, 0.529f, 1.0f); //RGBA: {0.871f, 0.722f, 0.529f, 1.0f}
		static const Color Tan = Color(0.824f, 0.706f, 0.549f, 1.0f); //RGBA: {0.824f, 0.706f, 0.549f, 1.0f}
		static const Color RosyBrown = Color(0.737f, 0.561f, 0.561f, 1.0f); //RGBA: {0.737f, 0.561f, 0.561f, 1.0f}
		static const Color SandyBrown = Color(0.957f, 0.643f, 0.376f, 1.0f); //RGBA: {0.957f, 0.643f, 0.376f, 1.0f}
		static const Color Goldenrod = Color(0.855f, 0.647f, 0.125f, 1.0f); //RGBA: {0.855f, 0.647f, 0.125f, 1.0f}
		static const Color DarkGoldenrod = Color(0.722f, 0.525f, 0.043f, 1.0f); //RGBA: {0.722f, 0.525f, 0.043f, 1.0f}
		static const Color Peru = Color(0.804f, 0.522f, 0.247f, 1.0f); //RGBA: {0.804f, 0.522f, 0.247f, 1.0f}
		static const Color Chocolate = Color(0.824f, 0.412f, 0.118f, 1.0f); //RGBA: {0.824f, 0.412f, 0.118f, 1.0f}
		static const Color SaddleBrown = Color(0.545f, 0.271f, 0.075f, 1.0f); //RGBA: {0.545f, 0.271f, 0.075f, 1.0f}
		static const Color Sienna = Color(0.627f, 0.322f, 0.176f, 1.0f); //RGBA: {0.627f, 0.322f, 0.176f, 1.0f}
		static const Color Brown = Color(0.647f, 0.165f, 0.165f, 1.0f); //RGBA: {0.647f, 0.165f, 0.165f, 1.0f}
		static const Color Maroon = Color(0.502f, 0.0f, 0.0f, 1.0f); //RGBA: {0.502f, 0.0f, 0.0f, 1.0f}
		static const Color White = Color(1.0f, 1.0f, 1.0f, 1.0f); //RGBA: {1.0f, 1.0f, 1.0f, 1.0f}
		static const Color Snow = Color(1.0f, 0.980f, 0.980f, 1.0f); //RGBA: {1.0f, 0.980f, 0.980f, 1.0f}
		static const Color Honeydew = Color(0.941f, 1.0f, 0.941f, 1.0f); //RGBA: {0.941f, 1.0f, 0.941f, 1.0f}
		static const Color MintCream = Color(0.961f, 1.0f, 0.980f, 1.0f); //RGBA: {0.961f, 1.0f, 0.980f, 1.0f}
		static const Color Azure = Color(0.941f, 1.0f, 1.0f, 1.0f); //RGBA: {0.941f, 1.0f, 1.0f, 1.0f}
		static const Color AliceBlue = Color(0.941f, 0.973f, 1.0f, 1.0f); //RGBA: {0.941f, 0.973f, 1.0f, 1.0f}
		static const Color GhostWhite = Color(0.973f, 0.973f, 1.0f, 1.0f); //RGBA: {0.973f, 0.973f, 1.0f, 1.0f}
		static const Color WhiteSmoke = Color(0.961f, 0.961f, 0.961f, 1.0f); //RGBA: {0.961f, 0.961f, 0.961f, 1.0f}
		static const Color SeaShell = Color(1.0f, 0.961f, 0.933f, 1.0f); //RGBA: {1.0f, 0.961f, 0.933f, 1.0f}
		static const Color Beige = Color(0.961f, 0.961f, 0.863f, 1.0f); //RGBA: {0.961f, 0.961f, 0.863f, 1.0f}
		static const Color OldLace = Color(0.992f, 0.961f, 0.902f, 1.0f); //RGBA: {0.992f, 0.961f, 0.902f, 1.0f}
		static const Color FloralWhite = Color(1.0f, 0.980f, 0.941f, 1.0f); //RGBA: {1.0f, 0.980f, 0.941f, 1.0f}
		static const Color Ivory = Color(1.0f, 1.0f, 0.941f, 1.0f); //RGBA: {1.0f, 1.0f, 0.941f, 1.0f}
		static const Color AntiqueWhite = Color(0.980f, 0.922f, 0.843f, 1.0f); //RGBA: {0.980f, 0.922f, 0.843f, 1.0f}
		static const Color Linen = Color(0.980f, 0.941f, 0.902f, 1.0f); //RGBA: {0.980f, 0.941f, 0.902f, 1.0f}
		static const Color LavenderBlush = Color(1.0f, 0.941f, 0.961f, 1.0f); //RGBA: {1.0f, 0.941f, 0.961f, 1.0f}
		static const Color MistyRose = Color(1.0f, 0.894f, 0.882f, 1.0f); //RGBA: {1.0f, 0.894f, 0.882f, 1.0f}
		static const Color Gainsboro = Color(0.863f, 0.863f, 0.863f, 1.0f); //RGBA: {0.863f, 0.863f, 0.863f, 1.0f}
		static const Color LightGrey = Color(0.827f, 0.827f, 0.827f, 1.0f); //RGBA: {0.827f, 0.827f, 0.827f, 1.0f}
		static const Color Silver = Color(0.753f, 0.753f, 0.753f, 1.0f); //RGBA: {0.753f, 0.753f, 0.753f, 1.0f}
		static const Color DarkGrey = Color(0.663f, 0.663f, 0.633f, 1.0f); //RGBA: {0.663f, 0.663f, 0.633f, 1.0f
		static const Color Gray = Color(0.502f, 0.502f, 0.502f, 1.0f); //RGBA: {0.502f, 0.502f, 0.502f, 1.0f}
		static const Color DimGray = Color(0.412f, 0.412f, 0.412f, 1.0f); //RGBA: {0.412f, 0.412f, 0.412f, 1.0f}
		static const Color LightSlateGray = Color(0.467f, 0.533f, 0.600f, 1.0f); //RGBA: {0.467f, 0.533f, 0.600f, 1.0f}
		static const Color SlateGray = Color(0.439f, 0.502f, 0.565f, 1.0f); //RGBA: {0.439f, 0.502f, 0.565f, 1.0f}
		static const Color DarkSlateGray = Color(0.184f, 0.310f, 0.310f, 1.0f); //RGBA: {0.184f, 0.310f, 0.310f, 1.0f}
		static const Color Black = Color(0.0f, 0.0f, 0.0f, 1.0f); //RGBA: {0.0f, 0.0f, 0.0f, 1.0f}
	}

	const Color Color::FromUString(UString colorStr)
	{
		using namespace Colors;

		if (colorStr == VTEXT("indianred")) {
			return IndianRed;
		}
		else if (colorStr == VTEXT("lightcoral")) {
			return LightCoral;
		}
		else if (colorStr == VTEXT("salmon")) {
			return Salmon;
		}
		else if (colorStr == VTEXT("darksalmon")) {
			return DarkSalmon;
		}
		else if (colorStr == VTEXT("lightsalmon")) {
			return LightSalmon;
		}
		else if (colorStr == VTEXT("crimson")) {
			return Crimson;
		}
		else if (colorStr == VTEXT("red")) {
			return Red;
		}
		else if (colorStr == VTEXT("firebrick")) {
			return FireBrick;
		}
		else if (colorStr == VTEXT("darkred")) {
			return DarkRed;
		}
		else if (colorStr == VTEXT("pink")) {
			return Pink;
		}
		else if (colorStr == VTEXT("lightpink")) {
			return LightPink;
		}
		else if (colorStr == VTEXT("hotpink")) {
			return HotPink;
		}
		else if (colorStr == VTEXT("deeppink")) {
			return DeepPink;
		}
		else if (colorStr == VTEXT("mediumvioletred")) {
			return MediumVioletRed;
		}
		else if (colorStr == VTEXT("palevioletred")) {
			return PaleVioletRed;
		}
		else if (colorStr == VTEXT("coral")) {
			return Coral;
		}
		else if (colorStr == VTEXT("tomato")) {
			return Tomato;
		}
		else if (colorStr == VTEXT("orangered")) {
			return OrangeRed;
		}
		else if (colorStr == VTEXT("darkorange")) {
			return DarkOrange;
		}
		else if (colorStr == VTEXT("orange")) {
			return Orange;
		}
		else if (colorStr == VTEXT("gold")) {
			return Gold;
		}
		else if (colorStr == VTEXT("yellow")) {
			return Yellow;
		}
		else if (colorStr == VTEXT("lightyellow")) {
			return LightYellow;
		}
		else if (colorStr == VTEXT("lemonchion")) {
			return LemonChion;
		}
		else if (colorStr == VTEXT("lightgoldenrodyellow")) {
			return LightGoldenRodYellow;
		}
		else if (colorStr == VTEXT("papayawhip")) {
			return PapayaWhip;
		}
		else if (colorStr == VTEXT("moccasin")) {
			return Moccasin;
		}
		else if (colorStr == VTEXT("peachpu")) {
			return PeachPu;
		}
		else if (colorStr == VTEXT("palegoldenrod")) {
			return PaleGoldenRod;
		}
		else if (colorStr == VTEXT("khaki")) {
			return Khaki;
		}
		else if (colorStr == VTEXT("darkkhaki")) {
			return DarkKhaki;
		}
		else if (colorStr == VTEXT("lavender")) {
			return Lavender;
		}
		else if (colorStr == VTEXT("thistle")) {
			return Thistle;
		}
		else if (colorStr == VTEXT("plum")) {
			return Plum;
		}
		else if (colorStr == VTEXT("violet")) {
			return Violet;
		}
		else if (colorStr == VTEXT("orchid")) {
			return Orchid;
		}
		else if (colorStr == VTEXT("magenta")) {
			return Magenta;
		}
		else if (colorStr == VTEXT("mediumorchid")) {
			return MediumOrchid;
		}
		else if (colorStr == VTEXT("mediumpurple")) {
			return MediumPurple;
		}
		else if (colorStr == VTEXT("blueviolet")) {
			return BlueViolet;
		}
		else if (colorStr == VTEXT("darkviolet")) {
			return DarkViolet;
		}
		else if (colorStr == VTEXT("darkorchid")) {
			return DarkOrchid;
		}
		else if (colorStr == VTEXT("darkmagenta")) {
			return DarkMagenta;
		}
		else if (colorStr == VTEXT("purple")) {
			return Purple;
		}
		else if (colorStr == VTEXT("indigo")) {
			return Indigo;
		}
		else if (colorStr == VTEXT("slateblue")) {
			return SlateBlue;
		}
		else if (colorStr == VTEXT("darkslateblue")) {
			return DarkSlateBlue;
		}
		else if (colorStr == VTEXT("greenyellow")) {
			return GreenYellow;
		}
		else if (colorStr == VTEXT("chartreuse")) {
			return Chartreuse;
		}
		else if (colorStr == VTEXT("lawngreen")) {
			return LawnGreen;
		}
		else if (colorStr == VTEXT("lime")) {
			return Lime;
		}
		else if (colorStr == VTEXT("limegreen")) {
			return LimeGreen;
		}
		else if (colorStr == VTEXT("palegreen")) {
			return PaleGreen;
		}
		else if (colorStr == VTEXT("lightgreen")) {
			return LightGreen;
		}
		else if (colorStr == VTEXT("mediumspringgreen")) {
			return MediumSpringGreen;
		}
		else if (colorStr == VTEXT("springgreen")) {
			return SpringGreen;
		}
		else if (colorStr == VTEXT("seagreen")) {
			return SeaGreen;
		}
		else if (colorStr == VTEXT("forestgreen")) {
			return ForestGreen;
		}
		else if (colorStr == VTEXT("green")) {
			return Green;
		}
		else if (colorStr == VTEXT("darkgreen")) {
			return DarkGreen;
		}
		else if (colorStr == VTEXT("yellowgreen")) {
			return YellowGreen;
		}
		else if (colorStr == VTEXT("olivedrab")) {
			return OliveDrab;
		}
		else if (colorStr == VTEXT("olive")) {
			return Olive;
		}
		else if (colorStr == VTEXT("darkolivegreen")) {
			return DarkOliveGreen;
		}
		else if (colorStr == VTEXT("mediumaquamarine")) {
			return MediumAquamarine;
		}
		else if (colorStr == VTEXT("darkseagreen")) {
			return DarkSeaGreen;
		}
		else if (colorStr == VTEXT("lightseagreen")) {
			return LightSeaGreen;
		}
		else if (colorStr == VTEXT("darkcyan")) {
			return DarkCyan;
		}
		else if (colorStr == VTEXT("teal")) {
			return Teal;
		}
		else if (colorStr == VTEXT("aqua")) {
			return Aqua;
		}
		else if (colorStr == VTEXT("cyan")) {
			return Cyan;
		}
		else if (colorStr == VTEXT("lightcyan")) {
			return LightCyan;
		}
		else if (colorStr == VTEXT("paleturquoise")) {
			return PaleTurquoise;
		}
		else if (colorStr == VTEXT("aquamarine")) {
			return Aquamarine;
		}
		else if (colorStr == VTEXT("turquoise")) {
			return Turquoise;
		}
		else if (colorStr == VTEXT("mediumturquoise")) {
			return MediumTurquoise;
		}
		else if (colorStr == VTEXT("darkturquoise")) {
			return DarkTurquoise;
		}
		else if (colorStr == VTEXT("cadetblue")) {
			return CadetBlue;
		}
		else if (colorStr == VTEXT("steelblue")) {
			return SteelBlue;
		}
		else if (colorStr == VTEXT("lightsteelblue")) {
			return LightSteelBlue;
		}
		else if (colorStr == VTEXT("lightblue")) {
			return LightBlue;
		}
		else if (colorStr == VTEXT("powderblue")) {
			return PowderBlue;
		}
		else if (colorStr == VTEXT("skyblue")) {
			return SkyBlue;
		}
		else if (colorStr == VTEXT("lightskyblue")) {
			return LightSkyBlue;
		}
		else if (colorStr == VTEXT("deepskyblue")) {
			return DeepSkyBlue;
		}
		else if (colorStr == VTEXT("dodgerblue")) {
			return DodgerBlue;
		}
		else if (colorStr == VTEXT("cornflowerblue")) {
			return CornflowerBlue;
		}
		else if (colorStr == VTEXT("mediumslateblue")) {
			return MediumSlateBlue;
		}
		else if (colorStr == VTEXT("royalblue")) {
			return RoyalBlue;
		}
		else if (colorStr == VTEXT("blue")) {
			return Blue;
		}
		else if (colorStr == VTEXT("mediumblue")) {
			return MediumBlue;
		}
		else if (colorStr == VTEXT("darkblue")) {
			return DarkBlue;
		}
		else if (colorStr == VTEXT("navy")) {
			return Navy;
		}
		else if (colorStr == VTEXT("midnightblue")) {
			return MidnightBlue;
		}
		else if (colorStr == VTEXT("cornsilk")) {
			return Cornsilk;
		}
		else if (colorStr == VTEXT("blanchedalmond")) {
			return BlanchedAlmond;
		}
		else if (colorStr == VTEXT("bisque")) {
			return Bisque;
		}
		else if (colorStr == VTEXT("navajowhite")) {
			return NavajoWhite;
		}
		else if (colorStr == VTEXT("wheat")) {
			return Wheat;
		}
		else if (colorStr == VTEXT("burlywood")) {
			return BurlyWood;
		}
		else if (colorStr == VTEXT("tan")) {
			return Tan;
		}
		else if (colorStr == VTEXT("rosybrown")) {
			return RosyBrown;
		}
		else if (colorStr == VTEXT("sandybrown")) {
			return SandyBrown;
		}
		else if (colorStr == VTEXT("goldenrod")) {
			return Goldenrod;
		}
		else if (colorStr == VTEXT("darkgoldenrod")) {
			return DarkGoldenrod;
		}
		else if (colorStr == VTEXT("peru")) {
			return Peru;
		}
		else if (colorStr == VTEXT("chocolate")) {
			return Chocolate;
		}
		else if (colorStr == VTEXT("saddlebrown")) {
			return SaddleBrown;
		}
		else if (colorStr == VTEXT("sienna")) {
			return Sienna;
		}
		else if (colorStr == VTEXT("brown")) {
			return Brown;
		}
		else if (colorStr == VTEXT("maroon")) {
			return Maroon;
		}
		else if (colorStr == VTEXT("white")) {
			return White;
		}
		else if (colorStr == VTEXT("snow")) {
			return Snow;
		}
		else if (colorStr == VTEXT("honeydew")) {
			return Honeydew;
		}
		else if (colorStr == VTEXT("mintcream")) {
			return MintCream;
		}
		else if (colorStr == VTEXT("azure")) {
			return Azure;
		}
		else if (colorStr == VTEXT("aliceblue")) {
			return AliceBlue;
		}
		else if (colorStr == VTEXT("ghostwhite")) {
			return GhostWhite;
		}
		else if (colorStr == VTEXT("whitesmoke")) {
			return WhiteSmoke;
		}
		else if (colorStr == VTEXT("seashell")) {
			return SeaShell;
		}
		else if (colorStr == VTEXT("beige")) {
			return Beige;
		}
		else if (colorStr == VTEXT("oldlace")) {
			return OldLace;
		}
		else if (colorStr == VTEXT("floralwhite")) {
			return FloralWhite;
		}
		else if (colorStr == VTEXT("ivory")) {
			return Ivory;
		}
		else if (colorStr == VTEXT("antiquewhite")) {
			return AntiqueWhite;
		}
		else if (colorStr == VTEXT("linen")) {
			return Linen;
		}
		else if (colorStr == VTEXT("lavenderblush")) {
			return LavenderBlush;
		}
		else if (colorStr == VTEXT("mistyrose")) {
			return MistyRose;
		}
		else if (colorStr == VTEXT("gainsboro")) {
			return Gainsboro;
		}
		else if (colorStr == VTEXT("lightgrey")) {
			return LightGrey;
		}
		else if (colorStr == VTEXT("silver")) {
			return Silver;
		}
		else if (colorStr == VTEXT("darkgrey")) {
			return DarkGrey;
		}
		else if (colorStr == VTEXT("gray")) {
			return Gray;
		}
		else if (colorStr == VTEXT("dimgray")) {
			return DimGray;
		}
		else if (colorStr == VTEXT("lightslategray")) {
			return LightSlateGray;
		}
		else if (colorStr == VTEXT("slategray")) {
			return SlateGray;
		}
		else if (colorStr == VTEXT("darkslategray")) {
			return DarkSlateGray;
		}
		else {
			return Black;
		}
	}

}

#endif
