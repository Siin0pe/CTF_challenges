import base64

encrypted_message = "dntyfkt1YWxkcnV2YnR2ZmB+fWZleX92c3x2fU0="

# NE PAS MODIFIER
def decrypt(message: str, key: str) -> str:
   buffer = base64.b64decode(message)
   plain_chars: list[str] = []

   for index, value in enumerate(buffer):
       char_code = value ^ ord(key[index % len(key)])
       plain_chars.append(chr(char_code))

   return "".join(plain_chars)


# TODO: boucler sur toutes les possibilités
    # TODO: construire la clé de 4 caractères
    # TODO: déchiffrer le message avec decrypt
    # TODO: si le message est déchiffré, affiché le
