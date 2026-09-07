"""Compile and execute the real portable firmware loader; no USB access."""
import ctypes
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

SOURCE = Path(__file__).resolve().parents[2] / "firmware/dpp_fw/main"
HEADER = b"DPIM\x01\x80\x75\x00"


class FirmwareProfileImageTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which("cc")
        if not compiler:
            raise unittest.SkipTest("C compiler unavailable")
        cls.build = tempfile.TemporaryDirectory()
        lib = Path(cls.build.name) / "loader.so"
        subprocess.run([compiler, "-std=c99", "-Wall", "-Wextra", "-Werror",
                        "-shared", "-fPIC", str(SOURCE / "profile_image.c"),
                        "-o", str(lib)], check=True, capture_output=True)
        cls.lib = ctypes.CDLL(str(lib))
        cls.callback_type = ctypes.CFUNCTYPE(None, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_bool)
        cls.lib.profile_image_draw.argtypes = [ctypes.c_char_p, ctypes.c_char_p,
                                               ctypes.c_bool, cls.callback_type]
        cls.lib.profile_image_draw.restype = ctypes.c_bool

    @classmethod
    def tearDownClass(cls):
        cls.build.cleanup()

    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.folder = self.root / "profile_TEST"
        self.folder.mkdir()
        self.file = self.folder / "profile-image.dppi"

    def draw(self, profile=b"TEST", landscape=False):
        pixels = []
        callback = self.callback_type(lambda x, y, white: pixels.append((x, y, white)))
        result = self.lib.profile_image_draw(str(self.root).encode(), profile, landscape, callback)
        return result, pixels

    def test_valid_bitmap_and_header_preservation(self):
        self.file.write_bytes(HEADER + b"\x80" + bytes(1870) + b"\x01")
        ok, pixels = self.draw()
        self.assertTrue(ok)
        self.assertEqual(len(pixels), 128 * 117)
        self.assertEqual([p for p in pixels if p[2]], [(0, 11, True), (127, 127, True)])
        self.assertTrue(all(11 <= y <= 127 for _, y, _ in pixels))

    def test_missing_truncated_extra_and_bad_headers_do_not_draw(self):
        self.assertEqual(self.draw(), (False, []))
        for data in (b"", HEADER[:7], HEADER + bytes(1871), HEADER + bytes(1873),
                     b"BAD!" + HEADER[4:] + bytes(1872),
                     HEADER[:4] + b"\x02" + HEADER[5:] + bytes(1872)):
            with self.subTest(size=len(data)):
                self.file.write_bytes(data)
                self.assertEqual(self.draw(), (False, []))

    def test_orientation_uses_separate_file_without_portrait_fallback(self):
        self.file.write_bytes(HEADER + bytes([255]) * 1872)
        self.assertEqual(self.draw(landscape=True), (False, []))
        (self.folder / "profile-image-landscape.dppi").write_bytes(HEADER + bytes(1872))
        ok, pixels = self.draw(landscape=True)
        self.assertTrue(ok)
        self.assertFalse(any(p[2] for p in pixels))

    def test_profile_switch_does_not_reuse_old_image(self):
        self.file.write_bytes(HEADER + bytes(1872))
        self.assertTrue(self.draw()[0])
        self.assertEqual(self.draw(b"OTHER"), (False, []))
        self.file.unlink()
        self.assertEqual(self.draw(), (False, []))

    def test_profile_path_validation(self):
        for name in (b"", b"x" * 17, b"../TEST", b"a/b", b"a\\b", b"a\n", b"\xff"):
            self.assertEqual(self.draw(name), (False, []))

    def test_all_header_fields_are_validated(self):
        for index in range(8):
            bad = bytearray(HEADER + bytes(1872))
            bad[index] ^= 1
            self.file.write_bytes(bad)
            self.assertEqual(self.draw(), (False, []))
