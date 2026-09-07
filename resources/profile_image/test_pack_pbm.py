import unittest
from pathlib import Path
from pack_pbm import HEADER, example_image, pack_pbm


class PackPBMTests(unittest.TestCase):
    def test_white_and_black_bit_order(self):
        pixels = ["1"] * (128 * 117)
        pixels[0] = pixels[-1] = "0"
        data = pack_pbm("P1\n# comment\n128 117\n" + " ".join(pixels))
        self.assertEqual(data, HEADER + b"\x80" + bytes(1870) + b"\x01")

    def test_invalid_input(self):
        full = "P1 128 117 " + "1 " * (128 * 117)
        for text in ("", full.replace("P1", "P4", 1), full.replace("117", "118", 1),
                     full + "0", full[:-2], full[:-2] + "2", " " * 65537):
            with self.assertRaises(ValueError):
                pack_pbm(text)

    def test_example_is_reproducible(self):
        data = example_image()
        self.assertEqual(len(data), 1880)
        self.assertEqual(data[:8], HEADER)
        self.assertEqual(data, Path(__file__).with_name("example.dppi").read_bytes())
